
class ListIterator : public std::iterator<std::bidirectional_iterator_tag, T> {
  private:
    // @TODO: graded in mp_lists part 1
    ListNode* position_;
    ListNode* tail;

  public:
    ListIterator() : position_(NULL), tail(NULL) { }
    ListIterator(ListNode* x) : position_(x) { 
        ListNode* temp = x;
        while (x != NULL){
            temp = x;
            x = x->next;
        }
        tail = temp;
    }
    ListIterator(ListNode* nul, ListNode* tail_) : position_(nul), tail(tail_) { }


    // Pre-Increment, ++iter
    ListIterator& operator++() {
        // @TODO: graded in mp_lists part 1
        if (position_ != NULL)
            position_ = position_->next;
        return *this;
    }
    
    // Post-Increment, iter++
    ListIterator operator++(int) {
        // @TODO: graded in mp_lists part 1
        ListNode * temp = NULL;
        if (position_ != NULL){
            temp = position_;
            position_ = position_->next;
        }
        return ListIterator(temp);
    }

    // Pre-Decrement, --iter
    ListIterator& operator--() {
        // @TODO: graded in mp_lists part 1
        if(position_ != NULL){
            position_ = position_->prev;
        } else {
            position_ = tail;
        }
        return *this;
    }

    // Post-Decrement, iter--
    ListIterator operator--(int) {
        // @TODO: graded in mp_lists part 1
        ListNode* temp = NULL;
        if (position_ != NULL) {
            temp = position_;
            position_ = position_->prev;
        } else{
            position_ = tail;
        }
        return ListIterator(temp);
    }

    bool operator!=(const ListIterator& rhs) {
        // @TODO: graded in mp_lists part 1
        return this->position_ != rhs.position_;
    }

    bool operator==(const ListIterator& rhs) {
        return !(*this != rhs);
    }

    const T& operator*() {
        return position_->data;
    }

    const T* operator->() {
        return &(position_->data);
    }
};
