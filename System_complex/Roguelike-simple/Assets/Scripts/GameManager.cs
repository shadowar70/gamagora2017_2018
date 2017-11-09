using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using System.Collections;
using System.Collections.Generic;


public class GameManager : MonoBehaviour {

	public float levelStartDelay = 2f;
	public float turnDelay = 0.1f;
	public static GameManager instance = null;
	public BoardManager boardScript;
	public int playerFoodPoints = 100;
	[HideInInspector] public bool playersTurn = true;

	private Text levelText;
	private GameObject levelImage;

	private int level = 0;
	private List<Enemy> enemies;
	private bool enemiesMoving;
	private bool doingSetup = true;

	void Awake (){
		if (instance == null) {
			instance = this;
		} else if (instance != this) {
			Destroy(gameObject);
		}

		DontDestroyOnLoad(gameObject);
		enemies = new List<Enemy> ();
		boardScript = GetComponent<BoardManager>();
        levelImage = GameObject.Find("LevelImage");
        //InitGame();
	}

	/*void OnLevelFinishedLoading(Scene scene, LoadSceneMode mode){
		level++;
		InitGame();
	}

	void OnEnable(){
		SceneManager.sceneLoaded += OnLevelFinishedLoading;
	}

	void OnDisable(){
		SceneManager.sceneLoaded -= OnLevelFinishedLoading;
	}*/

	//this is called only once, and the paramter tell it to be called only after the scene was loaded
	//(otherwise, our Scene Load callback would be called the very first load, and we don't want that)
	[RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.AfterSceneLoad)]
	static public void CallbackInitialization()
	{
		//register the callback to be called everytime the scene is loaded
		SceneManager.sceneLoaded += OnSceneLoaded;
	}

	//This is called each time a scene is loaded.
	static private void OnSceneLoaded(Scene arg0, LoadSceneMode arg1)
	{
        if (arg0.name == "Menu") {
            instance.level = 0;
        }
        else {
            instance.level++;
            instance.InitGame();
        }
        GameManager.instance.enabled = true;
    }

	void InitGame(){
		doingSetup = true;
        levelImage.SetActive(true);
		levelText = levelImage.GetComponentInChildren<Text>();
		levelText.text = "Floor " + level;
		levelImage.SetActive (true);
		Invoke ("HideLevelImage", levelStartDelay);

		enemies.Clear ();
		boardScript.SetupScene (level);
	}

	private void HideLevelImage(){
		levelImage.SetActive (false);
		doingSetup = false;
	}


	// Update is called once per frame
	void Update () {

		if (playersTurn || enemiesMoving || doingSetup)
			return;

		StartCoroutine (MoveEnemies ());

	}

	public void GameOver(){
		levelText.text = "You died after " + level + " floors.";
		levelImage.SetActive (true);
		enabled = false;
        
        Application.LoadLevel (0);

	}

	public void AddEnemyToList(Enemy script){
		enemies.Add (script);
	}


	IEnumerator MoveEnemies(){
		enemiesMoving = true;
		yield return new WaitForSeconds (turnDelay);
		if (enemies.Count == 0) {
			yield return new WaitForSeconds (turnDelay);
		}

		for (int i = 0; i < enemies.Count; i++) {
            //enemies [i].MoveEnemy ();
            enemies[i].AStarMoveEnemy(boardScript.nodePosition);
            yield return new WaitForSeconds (enemies [i].moveTime);
		}

		playersTurn = true;
		enemiesMoving = false;

	}



}
