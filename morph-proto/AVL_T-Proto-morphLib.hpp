
#ifndef RED_BLACK_T_RAW
#define RED_BLACK_T_RAW

#include <cstdint>
#include <memory>
#include <utility>
#include <iostream>
#include <cassert>
#include <optional>


/**
 * DATA STRUCTURE TYPE : TREE
 * IMPLEMENTATION TYPE : Binary Search Tree with AVL
 * CODE NAME : morph_proto
 * LANGUAGE : Written in C++ 17 with g++ compiler
 * ############################################ This program is a prototype ################################################
 * **THIS PROGRAM PROVIDES BASIC OPERATIONS OF AVL BINARY SEARCH TREE INSERTION AND DELETION
 */


namespace morph_proto{
    class AVL_T{
    private:
        struct Basic_struct{

            std::shared_ptr<Basic_struct> m_left;
            int m_key;
            int m_height;
            std::shared_ptr<Basic_struct> m_right;

            explicit
            Basic_struct(int key) noexcept 
                : m_left(nullptr), m_key(key), m_height(1), m_right(nullptr) {}
        }; 

        [[nodiscard]]
        auto M_Create_Node(int key) {
            return std::make_shared<Basic_struct>(key);
        }

        [[nodiscard]]
        int M_Calculate_Height(Basic_struct* node) noexcept{
            if (!node) return 0;

            int left_height = node->m_left ? node->m_left->m_height : 0;
            int right_height = node->m_right ? node->m_right->m_height : 0;

            return left_height < right_height ? right_height + 1 : left_height + 1;
        }

        [[nodiscard]]
        int M_Calculate_Balance_Factor(Basic_struct* node) noexcept{
            if (!node) return 0;

            int left_height = node->m_left ? node->m_left->m_height : 0;
            int right_height = node->m_right ? node->m_right->m_height : 0;

            return left_height - right_height;
        }

        [[nodiscard]]
        auto M_LLRot_R(std::shared_ptr<Basic_struct> node) noexcept {

            std::shared_ptr<Basic_struct> child_left {node->m_left};
            std::shared_ptr<Basic_struct> child_lRight {child_left->m_right};

            child_left->m_right =  node;
            std::cout << node.use_count() << '\n';
            node->m_left = child_lRight;

            node->m_height = M_Calculate_Height(node.get());  
            child_left->m_height = M_Calculate_Height(child_left.get());

            return child_left;     
        }

        [[nodiscard]]
        auto M_LRRot_R(std::shared_ptr<Basic_struct> node) noexcept{

            std::shared_ptr<Basic_struct> child_left = node->m_left;
            std::shared_ptr<Basic_struct> child_RLeft = child_left->m_right;

            child_left->m_right = child_RLeft->m_left;
            node->m_left = child_RLeft->m_right;

            child_RLeft->m_left = child_left;
            child_RLeft->m_right = node;

            child_left->m_height = M_Calculate_Height(child_left.get());
            node->m_height = M_Calculate_Height(node.get());
            child_RLeft->m_height = M_Calculate_Height(child_RLeft.get());

            return child_RLeft;
        }

        [[nodiscard]]
        auto M_RLRot_R(std::shared_ptr<Basic_struct> node) noexcept -> std::shared_ptr<Basic_struct> {

            std::shared_ptr<Basic_struct> child_right = node->m_right;
            std::shared_ptr<Basic_struct> child_rLeft = child_right->m_left;

            node->m_right = child_rLeft->m_left;
            child_right->m_left = child_rLeft->m_right;

            child_rLeft->m_left = node;
            child_rLeft->m_right = child_right;

            node->m_height = M_Calculate_Height(node.get());
            child_right->m_height = M_Calculate_Height(child_right.get());
            child_rLeft->m_height = M_Calculate_Height(child_rLeft.get());

            return child_rLeft;
        }

        [[nodiscard]]
        auto M_RRRot_R(std::shared_ptr<Basic_struct> node) noexcept -> std::shared_ptr<Basic_struct> {

            std::shared_ptr<Basic_struct> child_right = node->m_right;
            std::shared_ptr<Basic_struct> child_lRight = child_right->m_left;

            child_right->m_left = node;
            node->m_right = child_lRight;

            node->m_height = M_Calculate_Height(node.get());
            child_right->m_height = M_Calculate_Height(child_right.get());

            return child_right;
        }

        std::shared_ptr<Basic_struct> root;

        [[nodiscard]]
        auto M_Insert_Imp(std::shared_ptr<Basic_struct> node, int key) -> std::shared_ptr<Basic_struct> {
            if(!node){
                return M_Create_Node(key);
            }

            if(key > node->m_key){
                node->m_right = M_Insert_Imp(node->m_right,key);        
            }
            else if(key < node->m_key){
                node->m_left = M_Insert_Imp(node->m_left,key);
            }

            node->m_height = M_Calculate_Height(node.get());

            if(M_Calculate_Balance_Factor(node.get()) == 2 && M_Calculate_Balance_Factor(node->m_left.get()) == 1)
                return M_LLRot_R(node);
            if (M_Calculate_Balance_Factor(node.get()) == 2 && M_Calculate_Balance_Factor(node->m_left.get()) == -1)
                return M_LRRot_R(node);
            if(M_Calculate_Balance_Factor(node.get()) == -2 && M_Calculate_Balance_Factor(node->m_right.get()) == 1)
                return M_RLRot_R(node);
            if(M_Calculate_Balance_Factor(node.get()) == -2 && M_Calculate_Balance_Factor(node->m_right.get()) == -1)
                return M_RRRot_R(node);

            return node;
        }

        auto M_Deletion_Imp(std::shared_ptr<Basic_struct> node, int key) {
            if(!node)
                return std::shared_ptr<Basic_struct>(nullptr);
            if(node->m_key < key)
                node->m_right = M_Deletion_Imp(node->m_right,key);
            else if(node->m_key > key)
                node->m_left = M_Deletion_Imp(node->m_left,key);
            else{
                if(!node->m_left && !node->m_right)
                    return node->m_left;
                if(!node->m_left)
                    return node->m_right;
                if(!node->m_right)
                    return node->m_left;
                auto max_in_left = M_Max_Impl(node->m_left);
                node->m_key = max_in_left->m_key;  
                M_Deletion_Imp(node->m_left,max_in_left->m_key);
            }
            return node;
        }


        auto M_Max_Impl(std::shared_ptr<Basic_struct> node) const -> std::shared_ptr<Basic_struct> {
            Basic_struct* buff = node.get();
            if(!buff->m_right) return node; 
            while(buff){
                if(!buff->m_right->m_right)
                    return buff->m_right;
                buff = buff->m_right.get();
            }
            return std::shared_ptr<Basic_struct>(nullptr);
        }

        void M_Display_imp(Basic_struct* node) const noexcept{
            if(node){
                M_Display_imp(node->m_left.get());
                std::cout << node->m_key << ' ';
                M_Display_imp(node->m_right.get());
            }
        }

        
    public:

        AVL_T() = default;

        void insert(int key){
            root = M_Insert_Imp(root,key);
        }

        void erase(int key){
            if(!root) return;
            root = M_Deletion_Imp(root,key);
        }

        void display() const noexcept {
            if(!root) return;
            M_Display_imp(root.get());
        }

        int max() const {
            if(!root) return INT32_MIN;
            auto buff = M_Max_Impl(root->m_right);
            return buff ? buff->m_key : INT32_MAX;
        }
    };
}








#endif //Red_Black_trees_RAW