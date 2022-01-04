# include <stdio.h>
# include <stdlib.h>
#include <limits.h>
# define true -1
# define false -2


struct Node;
typedef struct Edge
{
    int weight;
    struct Node *dest;
    struct Edge *next;

}edge, *pedge;
typedef struct Node
{
    int id;
    pedge out;
    struct Node *next;
}node, *pnode;


/*********************** algo *************/
int findEdgeWeight(struct Node** graph, int u, int v) //find the edge from src to dest and return its weight. if not fount return -1.
{
    int dest = 0;
    pnode tempNode = *graph;
    for(int i=0; i<v; i++)
        tempNode = tempNode->next;
    dest = tempNode->id;

    pnode src = *graph;
    for(int i = 0; i < u; i++)
        src = src->next;
    pedge e = src->out;
    while(e != NULL)
    {
        if(e->dest->id == dest)
            return e->weight;
        e = e->next;
    }
    return -1;
}

int minDistance(const int dist[], const int visited[], int V)
{
    int min = INT_MAX, min_index;
    for (int v = 0; v < V; v++)
        if (visited[v] == false && dist[v] <= min)
        {
            min = dist[v];
            min_index = v;
        }
    return min_index;
}

int dijkstra(struct Node** graph, int s, int d)
{
    pnode temp = *graph;
    int size = 0;
    int i, u, v;
    int weight;
    int src=0, dest=0;
    while (temp != NULL) //check size of the graph nodes (and check the index of the src and dest nodes in the graph).
    {
        if(temp->id == s)
            src = size;
        if(temp->id == d)
            dest = size;
        temp = temp->next;
        size++;
    }
    int *dist = (int*)malloc(size * sizeof(int));
    int *visited = (int*)malloc(size * sizeof(int));
    for (i = 0; i < size; i++) //zero all arrays.
    {
        dist[i] = INT_MAX;
        visited[i] = false;
    }
    dist[src] = 0;
    for (i = 0; i < size; i++) //we don't check the dest from srs itself, so we can skip it.
    {
        u = minDistance(dist, visited, size);
        visited[u] = true;
        for (v = 0; v < size; v++)
        {
            weight = findEdgeWeight(graph, u, v);
            if ((visited[v] == false) && (weight != -1) && (dist[u] != INT_MAX) && (dist[u] + weight < dist[v]))
                dist[v] = dist[u] + weight;
        }
    }
    weight = dist[dest]; //final answer
    free(dist); //free memory
    free(visited);//free memory
    if(weight == INT_MAX) //if there is no path the value of weight will be INT_MAX, so we will return -1.
        return -1;
    return weight;
}

int TSP(struct Node** graph, int list[], int size)
{
    int i, j, z;
    int answer = INT_MAX, srcWeight, weight, tempWeight;
    int src, newSrc, newSrcIndex;
    int stop = false; //used to signal when to stop checking specific src route
    int *visited = (int*)malloc(size * sizeof(int));

    for(i=0; i<size; i++) //checks all the best paths from every node as a src. checks which one is the best.
    {
        for(j=0; j<size; j++) //zero all values to false.
            visited[j] = false;
        visited[i] = true;
        srcWeight = 0; //check the full weight of the path from a given src node
        src = list[i]; //change the src every loop run
        stop = false;
        for(j=0; j<size-1; j++)
        {
            weight = INT_MAX;
            for (z=0; z<size; z++) //check the best node to go to from current node
            {
                if(visited[z] == true) //node wont have an edge to itself so we will skip it.
                    continue;

                tempWeight = dijkstra(graph, src, list[z]);
                if(tempWeight == -1) //if we dont have a way to reach one of the nodes in the list then our current src is bad and we shoud move on.
                {
                    stop = true;
                    break;
                }
                if(tempWeight < weight) //search for the best node from the list to go to from our current node.
                {
                    weight = tempWeight;
                    newSrc = list[z];
                    newSrcIndex = z;
                }
            }
            if(stop == true)
                break;

            srcWeight = srcWeight + weight;
            src = newSrc;
            visited[newSrcIndex] = true;
        }
        if(stop == false && srcWeight < answer)
            answer = srcWeight;
    }
    if(answer == INT_MAX)
        return -1;
    free(visited);
    return answer;
}



/*********************** graph *************/
void newNode(struct Node** graph, int id) //add node to the graph
{
    if((*graph)->id == -1) //if the graph is empty then add the new node data to the graph header
    {
        (*graph)->id = id;
        return;
    }
    pnode new_node = (pnode)malloc(sizeof(node));
    new_node->id = id;
    new_node->out = NULL;
    new_node->next = *graph; //make new node first.
    *graph = new_node; //move the head to point to the new node
}

void removeNodeOutEdges(pnode current) //remove the out edges from a given node pointer.
{
    pedge tempEdge = NULL;
    while (current->out != NULL) //go over the out edges from the current node and free them all.
    {
        tempEdge = current->out;
        current->out = (current->out)->next;
        free(tempEdge);
    }
    current->out = NULL;
}

void removeAllEdgesToNode(struct Node** graph, int id) //remove all the edges in the graph that go in to a given node id
{
    pedge tempEdge = NULL;
    pedge edgeToRemove = NULL;
    pnode tempNode = *graph;
    while (tempNode != NULL)
    {
        if(tempNode->out == NULL || tempNode->id == id) //if the node doest have out edges, or we reached the node we want to remove, then move to the next node
        {
            tempNode = tempNode->next;
            continue;
        }
        if(tempNode->out->dest->id == id) //if the edge we need to delete is the first one, then no need to loop because node can't have to edges to the same dest.
        {
            tempEdge = tempNode->out;
            tempNode->out = tempNode->out->next;
            free(tempEdge);
            tempNode = tempNode->next;
            continue;
        }
        tempEdge = tempNode->out;
        while (tempEdge->next != NULL) //loop until we find edge to remove. if found remove it, break the loop and move on to the next node
        {
            if(tempEdge->next->dest->id == id)
            {
                edgeToRemove = tempEdge->next;
                tempEdge->next = tempEdge->next->next;
                free(edgeToRemove);
                break;
            }
            tempEdge = tempEdge->next;
        }
        tempNode = tempNode->next;
    }
}

void newEdge(struct Node** graph, int src, int dest, int weight) //add new edge to the graph.
{
    pnode first = *graph;
    while(first->id != src) //find src node
        first = first->next;

    pnode second = *graph;
    while(second != NULL) // look in the graph if we already have the dest node. if not we will create it.
    {
        if(second->id == dest)
            break;
        second = second->next;
    }
    if(second == NULL)
    {
        newNode(graph, dest);
        second = *graph;
    }

    pedge new_edge = (pedge)malloc(sizeof(edge));
    new_edge->dest = second;
    new_edge->weight = weight;
    new_edge->next = first->out;
    first->out = new_edge;
}

int main()
{
    int flag=false; //false means we don't have the new command input. true means we do have.
    int i, pathSrc=0, pathDest=0;
    int *list = NULL;
    int id=0, weight=0, sizeInput;
    char ch=0, command=0;
    pnode tempNode = NULL;
    pnode secondTempNode = NULL;

    pnode graph = (pnode) malloc(sizeof(node)); // create the graph (node serve as temp head for the nodes linked list).
    graph->id = -1;
    graph->out = NULL;
    graph->next = NULL;


    while(1)
    {   
        if(flag == false) //means we need to get new command.
        {
            scanf(" %c", &command);
            flag=true;
        }

        if(getchar()==EOF)
            break;

        if (command == 'A') //making new graph
        {
            flag = false;
            if (graph->id != -1) //check if we already had a grapg before, so we can delete it first.
            {
                while (graph != NULL) //go over all the nodes, remove them and the edges coming out of them.
                {
                    tempNode = graph;
                    removeNodeOutEdges(graph);
                    graph = graph->next;
                    free(tempNode);
                }
                graph = (pnode) malloc(sizeof(node)); // create the graph (node serve as temp head for the nodes linked list).
                graph->id = -1;
                graph->out = NULL;
                graph->next = NULL;
            }

            scanf(" %d", &sizeInput); //scan size of nodes in new graph.
            scanf(" %c", &ch); //scan first n sign
            for(i=0; i<sizeInput; i++)
            {
                scanf(" %d", &id); //scan the id of wanted node
                tempNode = graph;
                while(tempNode != NULL) // go over all the graph nodes and check if the wanted node already exist. if we found it then break the while loop.
                {
                    if(tempNode->id == id)
                        break;
                    tempNode = tempNode->next;
                }
                if(tempNode == NULL) // if we didn't find one the value of tempNode will be Null. so we insert new node.
                    newNode(&graph, id);
                while(1) // insert edges. stop if we reached input of n or different command (A, B, ...).
                {
                    scanf(" %c", &ch); // will be dest or 'n'
                    if(ch == 'n')
                        break;
                    if(ch == 'A' || ch == 'B' || ch == 'D' || ch == 'S' || ch == 'T')
                    {
                        flag = true;
                        command = ch;
                        break;
                    }
                    scanf(" %d", &weight); // has to be weight
                    newEdge(&graph, id, (int)ch - 48, weight);
                }
                if (flag == true) // if we have another command then move on.
                    break;
            }
        }

        if (command == 'B')  //making new vertex
        {
            flag = false;
            scanf(" %d", &id);
            tempNode = graph;
            while(tempNode != NULL) //check if we already have vertex with the same id
            {
                if(tempNode->id == id)
                    break;
                tempNode = tempNode->next;
            }
            if(tempNode == NULL) //if not found then create one
            {
                newNode(&graph, id);
            }
            else //if we found one delete its out nodes only
                removeNodeOutEdges(tempNode);

            while (1) //endless loop to add edges
            {
                scanf(" %c", &command);
                if(command == 'A' || command == 'B' || command == 'D' || command == 'S' || command == 'T')
                {
                    flag = true;
                    break;
                }
                scanf(" %d", &weight);
                newEdge(&graph, id, (int)command - 48, weight);
            }
        }

        if (command == 'D')  //deleting a vertex
        {
            flag = false;
            scanf(" %d", &id);
            removeAllEdgesToNode(&graph, id); //start by removing all edges to the node
            if(graph->id == id) // if the node is the first in the linked list remove it,
            {
                tempNode = graph;
                removeNodeOutEdges(graph);
                graph = graph->next;
                free(tempNode);
            }
            else // if the node is not the first, find it and then remove it.
            {
                tempNode = graph;
                while (tempNode->next->id != id)
                    tempNode = tempNode->next;
                removeNodeOutEdges(tempNode->next);
                secondTempNode = tempNode->next;
                tempNode->next = tempNode->next->next;
                free(secondTempNode);
            }
            scanf(" %c",&command); //accept new command
            flag = true;
        }

        if (command == 'S') //printing the shortest path
        {
            flag = false;
            scanf(" %d", &pathSrc);
            scanf(" %d", &pathDest);
            weight = dijkstra(&graph, pathSrc, pathDest);
            printf("Dijsktra shortest path: %d\n", weight);

            scanf(" %c", &command);
            flag = true;
        }

        if (command == 'T') // print TSP shortest path
        {
            flag = false;
            scanf(" %d", &sizeInput);
            list = (int*)malloc(sizeInput * sizeof(int));
            for(i=0; i<sizeInput; i++)
            {
                scanf(" %d", &id);
                list[i] = id;
            }
            weight = TSP(&graph, list, sizeInput);
            printf("TSP shortest path: %d\n", weight);
            free(list);

            scanf(" %c", &command);
            flag = true;
        }
    }

    while(graph != NULL) //we wull remove all the graph data. go over all the nodes, remove them and the edges coming out of them.
    {
        tempNode = graph;
        removeNodeOutEdges(graph);
        graph = graph->next;
        free(tempNode);
    }
    return 0;
}