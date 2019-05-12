using UnityEngine;
using UnityEngine.UI;

using System.Collections;

public class MinimapCameraScript : MonoBehaviour {

    Camera camera;
   // public Texture2D saveTexture;
    public RawImage image;

    // Use this for initialization
    void Start () {
        camera = GetComponent<Camera>();

    }
	
	// Update is called once per frame
	void Update () {

        camera.targetTexture = new RenderTexture((int)128, (int)128, 24, RenderTextureFormat.RGB565);
        Texture t = camera.targetTexture;
        t.wrapMode = TextureWrapMode.Clamp;
        image.texture = camera.targetTexture;
    }

    void OnGUI()
    {
        
    }
}
