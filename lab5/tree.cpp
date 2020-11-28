#include "tree.h"

TreeNode::TreeNode(NodeType nodeType, int lineno)
{
    this->nodeType = nodeType;
    line = lineno;
    childNum = 0;
    for (int i = 0; i < MAXChildNum; i++)
    {
        child[i] = nullptr;
    }
}

void TreeNode::addChild(TreeNode *t)
{
    if (childNum < MAXChildNum)
    {
        child[childNum] = t;
        childNum++;
    }
}

void TreeNode::addSibling(TreeNode *t){
    if(!sibling){
        sibling=t;
    }else{
        TreeNode *temp = sibling;
        while (temp->sibling)
        {
            temp=temp->sibling;
        }
        temp->sibling=t;
    }
}

int NodeNum=0;
void TreeNode::genNodeId(){
    nodeID=NodeNum++;
    int i=0;
    while(child[i]){
        child[i]->genNodeId();
        i++;
    }
    if(sibling) sibling->genNodeId();
}

void TreeNode::printNodeInfo(){
    printf("%-10d  %-10d  %-10d\n", nodeID, line, nodeType);
}

void TreeNode::printAST(){
    printNodeInfo();
    int i=0;
    while(child[i]){
        child[i]->printAST();
        i++;
    }
    if(sibling) sibling->printAST();
}