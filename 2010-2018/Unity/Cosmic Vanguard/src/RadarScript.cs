using UnityEngine;
using System.Collections;

public class RadarScript : MonoBehaviour {

    Animation animation;


    const float ratio = (50.0f / 160.0f);

    public float size;

	// Use this for initialization
	void Start () {

        animation = GetComponent<Animation>();

    }
	
	// Update is called once per frame
	void Update () {
        float sizeRatio = size * ratio;

        transform.localScale = new Vector3(sizeRatio, sizeRatio, 0);

        animation["idle"].speed = 0.2f;
    }
}
