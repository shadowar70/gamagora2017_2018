using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class Enemy : MovingObject {

	public int playerDamage;

	private Animator animator;
	private Transform target;
	private bool skipMove;
	public AudioClip enemyAttack1;
	public AudioClip enemyAttack2;

	// Use this for initialization
	protected override void Start () {
		GameManager.instance.AddEnemyToList (this);
		animator = GetComponent<Animator> ();
		target = GameObject.FindGameObjectWithTag ("Player").transform;
		base.Start ();
	}

	protected override void AttemptMove<T>(int xDir, int yDir){
		if (skipMove) {
			skipMove = false;
			return;
		}

		base.AttemptMove<T> (xDir, yDir);
		skipMove = true;

	}

	public void MoveEnemy(){
		int xDir = 0;
		int yDir = 0;

		if (Mathf.Abs (target.position.x - transform.position.x) < float.Epsilon) {
			yDir = target.position.y > transform.position.y ? 1 : -1;
		} else {
			xDir = target.position.x > transform.position.x ? 1 : -1;
		}

		AttemptMove<Player> (xDir, yDir);
	}

    public void AStarMoveEnemy(List<Node> nodePositionStart) {
        Debug.Log("COUCOU");
        int xDir = 0;
        int yDir = 0;
        Node directionEnemy, enemyNode = null, playerNode = null;
        List<Node> nodePosition = nodePositionStart;
        for (int i = 0; i < nodePosition.Count; i++) {
            //Enemy Position
            if (nodePosition[i].position.Equals(gameObject.transform.position)) {
                enemyNode = nodePosition[i];
                Debug.Log(" ENEMY!");
            }

            if (nodePosition[i].position.Equals(GameObject.FindGameObjectWithTag("Player").transform.position)) {
                playerNode = nodePosition[i];
                Debug.Log(" PLAYER!");
            }
        }
        Debug.Log(" T DEDANS ?");
        if (enemyNode != null && playerNode != null) {
            Debug.Log(" EH BAS NON !");
            directionEnemy = AStar(nodePosition, playerNode, enemyNode);
            xDir = (int)directionEnemy.position.x;
            yDir = (int)directionEnemy.position.y;
        }

        AttemptMove<Player>(xDir, yDir);
    }

    protected override void OnCantMove<T> (T component){
		Player hitPlayer = component as Player;
		animator.SetTrigger ("enemyAttack");
		hitPlayer.LoseFood (playerDamage);

		SoundManager.instance.RandomizeSfx (enemyAttack1, enemyAttack2);

	}


    List<Node> GetNeighbor(List<Node> nodePosition, Node nodeCurrent) {

        List<Node> neighbor = new List<Node>();
        Vector3 positionNeighbor = nodeCurrent.position;
        Vector3 posTop, posDown, posLeft, posRight;
        posRight = new Vector3(positionNeighbor.x+1, positionNeighbor.y, positionNeighbor.z);
        posLeft = new Vector3(positionNeighbor.x - 1, positionNeighbor.y, positionNeighbor.z);
        posTop = new Vector3(positionNeighbor.x, positionNeighbor.y+1, positionNeighbor.z);
        posDown = new Vector3(positionNeighbor.x, positionNeighbor.y - 1, positionNeighbor.z);
        for (int i = 0; i < nodePosition.Count; i++) {
            if (nodePosition[i].walkable) {
                if (nodePosition[i].position.Equals(posRight)) {
                    neighbor.Add(nodePosition[i]);
                }
                if (nodePosition[i].position.Equals(posLeft)) {
                    neighbor.Add(nodePosition[i]);
                }
                if (nodePosition[i].position.Equals(posTop)) {
                    neighbor.Add(nodePosition[i]);
                }
                if (nodePosition[i].position.Equals(posDown)) {
                    neighbor.Add(nodePosition[i]);
                }
            }

        }

        return neighbor;

    }

    public int DistanceBetween(Node n1, Node n2) {

        //Horizontal
        int distHorizon = Mathf.Abs((int)n1.position.x - (int)n2.position.x);
        //Vertical
        int distVerti = Mathf.Abs((int)n1.position.y - (int)n2.position.y);

        return distVerti+distHorizon;
        
    }


    public Node AStar(List<Node> nodePosition, Node playerPosition, Node startPosition) {
        Debug.Log("Cible: "+ playerPosition.position+ " Depart: "+ startPosition.position);
        List<Node> closedList = new List<Node>();
        startPosition.cost = 0;
        startPosition.heuristique = DistanceBetween(playerPosition, startPosition);
        List<Node> openList = new List<Node>() { startPosition };
        List<Node> cameFromList = new List<Node>();
        List<Node> neighborList = new List<Node>();
        int tentative_cost = 0; //(Tentative_gScore)
        Node currentNode;
        currentNode = openList[0];
        while (openList.Count != 0) {

            //Target is find
            if (currentNode.ComparePosNode(playerPosition)) {
                //return reconstruct_path(cameFromList, currentNode);
                cameFromList.Add(currentNode);
                Debug.Log("Chemin Disponible! ");
                for (int i = 0; i < cameFromList.Count; i++) {
                    Debug.Log(cameFromList[i].position);
                }
                Debug.Log("FIN Chemin ! ");
                return cameFromList[1];
            }

            for(int i=0; i < openList.Count; i++) {

                //Si on a un voisin avec un cout inferieur ou egal
                if (currentNode.totalCost >= openList[i].totalCost) {
                    currentNode = openList[i];
                }

                openList.Remove(currentNode);
                closedList.Add(currentNode);
                neighborList = GetNeighbor(nodePosition, currentNode);

                //Traitement des voisin de la node current
                foreach (Node currentNeighbor in neighborList) {
                    //Si on a pas deja traite cette node
                    if (!closedList.Contains(currentNeighbor)) {

                        if (!openList.Contains(currentNeighbor)) {
                            openList.Add(currentNeighbor);
                            Debug.Log(" NEW VOISIN: "+ currentNeighbor.position);
                        }

                        tentative_cost = currentNode.cost + DistanceBetween(currentNode, currentNeighbor);
                        Debug.Log(" NEW COST is " + (tentative_cost < currentNeighbor.cost) + ". tentative cost: " + tentative_cost +" neighbor cost: "+ currentNeighbor.cost);
                        if (tentative_cost < currentNeighbor.cost) {
                          
                            if (!cameFromList.Contains(currentNode)) {
                                cameFromList.Add(currentNode);
                                Debug.Log("CHEMIN PLUS GRAND");
                            }
                            
                            for (int j = 0; j < nodePosition.Count; j++) {
                                if (nodePosition[j].Equals(currentNeighbor)) {
                                    nodePosition[j].cost = tentative_cost;
                                    nodePosition[j].heuristique = DistanceBetween(currentNeighbor, playerPosition);
                                    Debug.Log("MAJ NODE GRID: "+ nodePosition[j].position +" cost: "+ nodePosition[j].cost + " heuristique: "+ nodePosition[j].heuristique);
                                }
                            }
                        }
                    }

                }
                


            }


        }

        Debug.Log("Pas de chemin !");
        return startPosition;
        //C LE DRAME, IL A RIEN TROUVE

    }

}
