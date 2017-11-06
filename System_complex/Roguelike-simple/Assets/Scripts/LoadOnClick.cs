using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LoadOnClick : MonoBehaviour {

	public GameObject loading;

	public void LoadScene(int level){
		loading.SetActive (true);
		Application.LoadLevel (level);
	}
}
