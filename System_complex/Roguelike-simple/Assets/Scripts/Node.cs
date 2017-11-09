using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;

public class Node {

    public Vector3 position;
    public bool walkable;
    public int heuristique; //Distance case->cible 
    public int cost; //Cout du "terrain" (gScore/gcost)

    public Node(Vector3 pos, bool w, int c) {
        position = pos;
        walkable = w;
        cost = c;
    }

    //Ceci est le FCOST/Fscore
    public float totalCost {
        get { return cost + heuristique; }
    }

    public int CompareCostNode(Node n2) {

        if (heuristique > n2.heuristique) {
            return 1;
        }
        if (heuristique == n2.heuristique) {
            return 0;
        }

        return -1;
    }

    public bool ComparePosNode(Node n2) {

        if (position.x == n2.position.x && position.y == n2.position.y) {
            return true;
        }

        return false;
    }

    public void SetPosition(int xNew, int yNew, int zNew) {
        position.x = xNew;
        position.y = yNew;
        position.z = zNew;
    }

    // Use this for initialization
    void Start() {

    }

    // Update is called once per frame
    void Update() {

    }
}