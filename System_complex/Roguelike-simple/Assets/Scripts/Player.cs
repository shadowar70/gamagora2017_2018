using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class Player : MovingObject {

	public int wallDamage = 1;
	public int pointsPerFood = 20;
	public int pointsPerSoda = 10;
	public float restartLevelDelay = 1f;

	public Text foodText;
	public AudioClip moveSound1;
	public AudioClip moveSound2;
	public AudioClip eatSound1;
	public AudioClip eatSound2;
	public AudioClip drinkSound1;
	public AudioClip drinkSound2;
	public AudioClip gameOverSound;

	private Animator animator;
	private int food;

	// Use this for initialization
	protected override void Start () {
		animator = GetComponent<Animator> ();
		food = GameManager.instance.playerFoodPoints;
        foodText = GameObject.Find("FoodText").GetComponent<Text>();
        foodText.text = "Adam: " + food;
		base.Start ();
	}

	private void OnDisable(){
		//GameManager.instance.playerFoodPoints = food;
	}

	// Update is called once per frame
	void Update () {
		if(!GameManager.instance.playersTurn) return;

		int horizontal = 0;
		int vertical = 0;

		horizontal = (int) (Input.GetAxisRaw ("Horizontal"));
		vertical = (int) (Input.GetAxisRaw ("Vertical"));

		if (horizontal != 0)
			vertical = 0;

		if (horizontal != 0 || vertical != 0)
			AttemptMove<Wall> (horizontal, vertical);

	}

	protected override void AttemptMove<T>(int xDir, int yDir){
		food--;
		foodText.text = "Adam: " + food;
		base.AttemptMove<T> (xDir, yDir);
		RaycastHit2D hit;

		if (Move (xDir, yDir, out hit)) {
			SoundManager.instance.RandomizeSfx (moveSound1, moveSound2);
		}

		CheckIfGameOver ();
		GameManager.instance.playersTurn = false;

	}

	protected override void OnCantMove<T>(T component){
		Wall hitWall = component as Wall;
		hitWall.DamageWall (wallDamage);
		animator.SetTrigger ("playerChop");

	}

	private void OnTriggerEnter2D(Collider2D other){
		if (other.tag == "Exit") {
            GameManager.instance.playerFoodPoints = food;
            Invoke ("Restart", restartLevelDelay);
			enabled = false;
		} else if (other.tag == "Food") {
			food += pointsPerFood;
			foodText.text = "+" + pointsPerFood + " Adam: " + food;
			SoundManager.instance.RandomizeSfx (eatSound1, eatSound2);
			other.gameObject.SetActive (false);
		}else if (other.tag == "Soda") {
			food += pointsPerSoda;
			foodText.text = "+" + pointsPerSoda + " Adam: " + food;
			SoundManager.instance.RandomizeSfx (drinkSound1, drinkSound2);
			other.gameObject.SetActive (false);
		}

	}
		

	private void Restart(){
		SceneManager.LoadScene (1);
    }

	public void LoseFood(int loss){
		animator.SetTrigger ("playerHit");
		food -= loss;
		foodText.text = "-" + loss + " Adam: " + food;
		CheckIfGameOver ();

	}


	private void CheckIfGameOver(){
		if (food <= 0) {
			SoundManager.instance.PlaySingle (gameOverSound);
			SoundManager.instance.musicSource.Stop ();
            GameManager.instance.GameOver ();
            Invoke("RetourMenu", 1);
            Destroy(SoundManager.instance);
            DestroyObject(SoundManager.instance);
        }

	}

    public int getFood() {
        return food;
    }

    void RetourMenu() {
        SceneManager.LoadScene(0);
    }
}
