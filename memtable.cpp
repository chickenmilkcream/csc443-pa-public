#include "memtable.h"

#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

AVLNode::AVLNode(db_key_t key, db_val_t val) {
  this->key = key;
  this->val = val;
  this->left = NULL;
  this->right = NULL;
  this->height = 1;
}

AVLNode *AVLNode::put(db_key_t key, db_val_t val) {
  if (this->key > key) {
    if (!this->left) {
      this->left = new AVLNode(key, val);
    } else {
      this->left = this->left->put(key, val);
    }
  } else if (this->key < key) {
    if (!this->right) {
      this->right = new AVLNode(key, val);
    } else {
      this->right = this->right->put(key, val);
    }
  } else {
    this->val = val;  // already exists
    return this;
  }

  this->height = max(this->left ? this->left->height : 0,
                     this->right ? this->right->height : 0) +
                 1;

  int balance = (this->left ? this->left->height : 0) -
                (this->right ? this->right->height : 0);

  if (balance < -1 && this->right->key < key) {
    return this->rotate_left();
  }
  if (balance < -1 && this->right->key > key) {
    this->right = this->right->rotate_right();
    return this->rotate_left();
  }
  if (balance > 1 && this->left->key > key) {
    return this->rotate_right();
  }
  if (balance > 1 && this->left->key < key) {
    this->left = this->left->rotate_left();
    return this->rotate_right();
  }

  return this;
}

db_val_t AVLNode::get(db_key_t key) {
  if (this->key > key) {
    return this->left->get(key);
  }
  if (this->key < key) {
    return this->right->get(key);
  }
  return this->val;
}

void AVLNode::scan(db_key_t min_key, db_key_t max_key,
                   vector<pair<db_key_t, db_val_t>> &pairs) {
  if (this->key > min_key) {
    if (this->left) {
      this->left->scan(min_key, max_key, pairs);
    }
  }
  if (this->key >= min_key && this->key <= max_key) {
    pair<db_key_t, db_val_t> pair;
    pair = make_pair(this->key, this->val);
    pairs.push_back(pair);
  }
  if (this->key < max_key) {
    if (this->right) {
      this->right->scan(min_key, max_key, pairs);
    }
  }
}

AVLNode *AVLNode::rotate_left() {
  AVLNode *right = this->right;
  this->right = right->left;
  right->left = this;
  this->height = max(this->left ? this->left->height : 0,
                     this->right ? this->right->height : 0) +
                 1;
  right->height =
      max(this->height, right->right ? right->right->height : 0) + 1;
  return right;
}

AVLNode *AVLNode::rotate_right() {
  AVLNode *left = this->left;
  this->left = left->right;
  left->right = this;
  this->height = max(this->left ? this->left->height : 0,
                     this->right ? this->right->height : 0) +
                 1;
  right->height = max(left->left ? left->left->height : 0, this->height) + 1;
  return left;
}

AVLTree::AVLTree() { this->root = NULL; }

void AVLTree::put(db_key_t key, db_val_t val) {
  if (!this->root) {
    this->root = new AVLNode(key, val);
  } else {
    this->root->put(key, val);
  }
}

db_val_t AVLTree::get(db_key_t key) {
  if (this->root) {
    if (this->root->key == key) {
      return this->root->val;
    }
    return this->root->get(key);
  }
}

vector<pair<db_key_t, db_val_t>> AVLTree::scan(db_key_t min_key,
                                               db_key_t max_key) {
  vector<pair<db_key_t, db_val_t>> pairs;
  if (this->root) {
    this->root->scan(min_key, max_key, pairs);
  }
  return pairs;
}

Memtable::Memtable(unsigned int memtable_size) {
  this->max_size = memtable_size;
  this->size = 0;
  this->tree = AVLTree();
}

void Memtable::put(db_key_t key, db_val_t val) { this->tree.put(key, val); }

db_val_t Memtable::get(db_key_t key) { return this->tree.get(key); }

vector<pair<db_key_t, db_val_t>> Memtable::scan(db_key_t min_key,
                                                db_key_t max_key) {
  return this->tree.scan(min_key, max_key);
}

int main() {
  // test cases
  Memtable mt = Memtable(108);
  mt.put(5, 1);
  mt.put(6, 3);
  mt.put(2, 8);
  mt.put(1, 9);
  mt.put(1, 7);
  mt.put(9, 8);

  // cout << mt.get(5) << endl;
  // cout << mt.get(2) << endl;
  // cout << mt.get(1) << endl;
  // cout << mt.get(9) << endl;
  // cout << mt.get(6) << endl;

  vector<pair<db_key_t, db_val_t>> pairs = mt.scan(0, 999);
  cout << "size: " << pairs.size() << endl;
  for (pair<db_key_t, db_val_t> pair : pairs) {
    cout << pair.first << ", " << pair.second << endl;
  }
}