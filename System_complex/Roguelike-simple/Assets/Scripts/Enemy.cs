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

    public void AStarMoveEnemy() {
        int xDir = 0;
        int yDir = 0;

        if (Mathf.Abs(target.position.x - transform.position.x) < float.Epsilon) {
            yDir = target.position.y > transform.position.y ? 1 : -1;
        }
        else {
            xDir = target.position.x > transform.position.x ? 1 : -1;
        }

        AttemptMove<Player>(xDir, yDir);
    }

    protected override void OnCantMove<T> (T component){
		Player hitPlayer = component as Player;
		animator.SetTrigger ("enemyAttack");
		hitPlayer.LoseFood (playerDamage);

		SoundManager.instance.RandomizeSfx (enemyAttack1, enemyAttack2);

	}


    public bool AStar(List<Node> nodePosition, Node playerPosition, Node startPosition) {
        List<Node> closedList = new List<Node>();
        List<Node> openList = new List<Node>() { startPosition };
        Node currentNode;
        currentNode = openList[0];
        while (openList.Count != 0) {

            if (currentNode.ComparePosNode(playerPosition)) {
                return true;
            }

            foreach(Node n in openList) {
                if (currentNode.totalCost >= n.totalCost) {
                    currentNode = n;
                }
                




            }


        }

        //C LE DRAME, IL A RIEN TROUVE

    }

}
