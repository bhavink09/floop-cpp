#pragma once
#include <expected>
#include <memory>
#include <unordered_map>
#include <vector>

#include <node.hpp>

namespace floop {

enum class EngineError {
  invalid_input,
};

using EngineResult = std::expected<void, EngineError>;

class Engine {
public:
  Engine() = default;
  Engine(const Engine &) = delete;
  Engine &operator=(Engine &) = delete;
  Engine(Engine &&) = default;
  Engine &operator=(Engine &&) = default;

  void add_node(std::shared_ptr<Node> &&node_);
  EngineResult run();

private:
  std::unordered_map<Node::NodeId, std::shared_ptr<Node>> _node_map;
  // Topologicall sorted graph, created at startup
  std::vector<std::shared_ptr<Node>> _graph_list;
};
} // namespace floop
