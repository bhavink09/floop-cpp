#pragma once
#include <string>
#include <vector>

namespace floop {

class Node {
public:
  using NodeId = uint64_t;
  explicit Node(std::string &&name_)
      : _id(_generator++), _name(std::move(name_)) {}

  Node() = delete;

  inline std::string_view name() { return _name; }

  inline NodeId id() { return _id; }

  virtual void compute() = 0;
  virtual std::vector<NodeId> input_ids() = 0;
  virtual ~Node() = default;

  virtual void post_compute() {}

private:
  NodeId _id;
  std::string _name;
  static NodeId _generator;
};

template <typename T>
class Output {
public:
  inline bool is_dirty() const { return _is_dirty; }
  inline void set_dirty(bool flag = true) { _is_dirty = flag; }

  inline const T &data() const { return _data; }

  inline T &rw_data() {
    _is_dirty = true;
    return _data;
  }

  Output &operator=(const T &val_) {
    _is_dirty = true;
    _data = val_;
    return *this;
  }

private:
  bool _is_dirty;
  T _data;
};

template <typename T>
class OutputNode : public Node {
public:
  OutputNode() = delete;
  explicit OutputNode(std::string &&name_) : Node(std::move(name_)) {}
  virtual ~OutputNode() = default;

  inline const Output<T> &output() const { return _output; }
  inline bool is_dirty() const { return _output.is_dirty(); }

  virtual void post_compute() { _output.set_dirty(false); }

protected:
  Output<T> _output;
};
} // namespace floop
