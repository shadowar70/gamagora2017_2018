using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;

public class Node {

    public Vector3 position;
    public bool walkable;
    public int heuristique; //Distance case->cible
    public int cost; //Cout du "terrain"

    public Node(Vector3 pos, bool w, int c) {
        position = pos;
        walkable = w;
        cost = c;
    }

    // Use this for initialization
    void Start() {

    }

    // Update is called once per frame
    void Update() {

    }
}