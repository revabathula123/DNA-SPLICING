#include "dna_strand.hpp"
#include <stdexcept>
#include "node.hpp"




// dna_strand.cc





// Private helper function to deallocate nodes

// Destructor
DNAstrand::~DNAstrand() {
    // Call DeallocateNodes to free the memory
    DeallocateNodes(head_);
    tail_ = nullptr;
    head_ = nullptr;
}

// Private helper function to deallocate nodes
void DNAstrand::DeallocateNodes(Node* start_node) {
    Node* current = start_node;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }
}

// Private helper function to find the last occurrence of a pattern
Node* DNAstrand::FindLastOccurrence(const char* pattern, bool& first) {
    Node* last_occurrence = nullptr;
    Node* current = head_;
    Node* current2 = head_;
    first = false;
    while (current != nullptr) {
        const char* p = pattern;
        Node* temp = current;
        int i = 0;
        while (*p != '\0' && temp != nullptr && temp->data == *p) {
            ++p;
            ++i;
            temp = temp->next;
        }
        if (*p == '\0') {
            last_occurrence = current;
            for(int k=0;k<i;k++)
            {
                current = current->next;
            }
        }
        else
        {   current = current->next;
        }
    }
    if((last_occurrence != nullptr) &&(last_occurrence->next == head_->next))
    {   first = true;
        return last_occurrence;
    }
    else if(last_occurrence == nullptr)
    {
        return last_occurrence;
    }
    else
    {   while ((current2 != nullptr)&&(current2->next != last_occurrence)) 
        {
        current2 = current2->next;
        }
        return current2;
    }
}


void DNAstrand::SpliceIn(const char* pattern, DNAstrand& to_splice_in) {
   if (pattern == nullptr || pattern[0] == '\0' || to_splice_in.head_ == nullptr || &to_splice_in == this) {
       return;  // No match or invalid arguments
   }
   bool first = false;
   Node* last_occurrence = FindLastOccurrence(pattern,first);
   if (last_occurrence == nullptr) {
       throw std::runtime_error("Pattern not found in sequence");
   }
   if(first)    {
       Node* tail_after_last_occurrence = last_occurrence;
       int i =0;
       while(pattern[i+1]!='\0' && tail_after_last_occurrence != nullptr )
       {
       tail_after_last_occurrence = tail_after_last_occurrence->next;
       ++i;
       }
       to_splice_in.tail_->next = tail_after_last_occurrence->next;
       tail_after_last_occurrence->next = nullptr;
       DeallocateNodes(last_occurrence);
       head_= to_splice_in.head_;
   }
   else     {
       Node* tail_after_last_occurrence = last_occurrence->next;
       int i =0;
       while(pattern[i+1]!='\0' && tail_after_last_occurrence != nullptr )
       {
       tail_after_last_occurrence = tail_after_last_occurrence->next;
       ++i;
       }
       to_splice_in.tail_->next = tail_after_last_occurrence->next;
       tail_after_last_occurrence->next = nullptr;
       DeallocateNodes(last_occurrence->next);
       last_occurrence->next = to_splice_in.head_;
       tail_ = to_splice_in.tail_;
   }
   to_splice_in.head_ = nullptr;
   to_splice_in.tail_ = nullptr;
}

