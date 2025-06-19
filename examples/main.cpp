#include <engine.hpp>
#include <memory>
#include <node.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <vector>

using namespace floop;

class MarketDataNode : public OutputNode<uint32_t> {
public:
  MarketDataNode() : OutputNode("market_data") {}
  virtual void compute() override {
    _output = std::rand();
    spdlog::info("generated output:{}", _output.data());
  }

  virtual std::vector<NodeId> input_ids() override { return {}; }
};

class QuoterNode : public OutputNode<double> {
public:
  QuoterNode(std::shared_ptr<MarketDataNode> md_node_)
      : OutputNode("quoter"), _market_data(std::move(md_node_)) {}

  virtual void compute() override {
    if (_market_data->is_dirty()) {
      spdlog::info("compute quoter - md:{}", _market_data->output().data());
    }
  }

  virtual std::vector<NodeId> input_ids() override {
    return {_market_data->id()};
  }

private:
  std::shared_ptr<MarketDataNode> _market_data;
};

int main() {
  auto console = spdlog::stdout_color_mt("console");
  spdlog::set_default_logger(console);
  spdlog::set_level(spdlog::level::info);

  floop::Engine e;
  auto md_node = std::make_shared<MarketDataNode>();
  auto q_node = std::make_shared<QuoterNode>(md_node);
  e.add_node(std::move(md_node));
  e.add_node(std::move(q_node));
  e.run();
  return 0;
}
