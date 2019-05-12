using UnityEngine;
using System.Collections;

public class NovaScript : MonoBehaviour {
    public float lifeTime;
    
    public float damage;

    public float affectSpeed;
    public float affectDuration;

    public float ScaleTime;         // Scaling speed
    public Vector3 ScaleSize;       // The size wave will be scaled to

    new Transform transform;      
    MeshRenderer meshRenderer;     

    Color defaultColor;            
    Color color;                   

    int tintColorRef;
    float initialOffset;        // Initial UV offset 

    public float UVTime = -6f;            // UV Animation speed

    // Use this for initialization
    void Start () {
        transform = GetComponent<Transform>();
        meshRenderer = GetComponent<MeshRenderer>();

        tintColorRef = Shader.PropertyToID("_TintColor");
        defaultColor = meshRenderer.material.GetColor(tintColorRef);

        initialOffset = Random.Range(0f, 5f);
    }
	
	// Update is called once per frame
	void Update () {

        meshRenderer.material.SetTextureOffset("_MainTex", new Vector2(Time.time * UVTime + initialOffset, 0f));

        transform.localScale = Vector3.Slerp(transform.localScale, ScaleSize, Time.deltaTime * ScaleTime);
        if(transform.localScale.x > ScaleSize.x * 0.95f)
        {
            transform.localScale = ScaleSize;
            Destroy(this.gameObject);
        }
    }

    void OnTriggerEnter(Collider other)
    {

        if (other.tag == "Enemy")
        {

            other.gameObject.transform.parent = null;


            if (affectDuration > 0)
            {
                other.gameObject.SendMessage("doSlow", affectSpeed);
                other.gameObject.SendMessage("doSlowDuration", affectDuration);
            }

            other.gameObject.SendMessage("doDamage", damage);

        }
    }

}
