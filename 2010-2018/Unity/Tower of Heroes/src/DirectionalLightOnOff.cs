using UnityEngine;
using System.Collections;

public class DirectionalLightOnOff : MonoBehaviour {

    public bool bIsOn = true;
    public float fMinIntensity = 0.2f;
    public float fMaxIntensity = 1.0f;

    private float fIntensityDelta = 0.5f;
    private Light light = null;

	// Use this for initialization
	void Start () {
        light = this.gameObject.GetComponent<Light>();

    }
	
	// Update is called once per frame
	void Update () {
	    if (bIsOn == false)
        {
            if ((light.intensity -= (fIntensityDelta * Time.deltaTime)) < fMinIntensity)
            {
                light.intensity = fMinIntensity;
            }
        }
        else
        {
            if ((light.intensity += (fIntensityDelta * Time.deltaTime)) > fMaxIntensity)
            {
                light.intensity = fMaxIntensity;
            }
        }
	}
}
