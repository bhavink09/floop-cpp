#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>
#include <ranges>
#include <spdlog/spdlog.h>

#include <floop/engine.hpp>
#include <floop/node.hpp>

namespace floop {

void Engine::add_node(std::shared_ptr<Node> &&node_) {
  _node_map.emplace(node_->id(), std::move(node_));
}

auto Engine::run() -> EngineResult {
  if (_node_map.empty()) {
    return std::unexpected(EngineError::invalid_input);
  }

  typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
                                uint64_t>
      Graph;
  typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
  typedef std::pair<Node::NodeId, Node::NodeId> Pair;

  std::vector<Pair> edges;
  for (auto node_val : _node_map) {
    auto node_id = node_val.second->id();
    for (auto ip_id : node_val.second->input_ids()) {
      edges.emplace_back(ip_id, node_id);
    }
  }

  Graph g(edges.begin(), edges.end(), _node_map.size());
  std::vector<Vertex> container;
  boost::topological_sort(g, std::back_inserter(container));
  for (auto c : std::ranges::reverse_view(container)) {
    _graph_list.emplace_back(_node_map[c]);
  }

  while (1) {
    for (auto &node : _graph_list) {
      node->compute();
    }

    for (auto &node : _graph_list) {
      node->post_compute();
    }
  }
  return EngineResult{};
}
} // namespace floop
