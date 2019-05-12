using UnityEngine;
using System.Collections;

[RequireComponent(typeof(LineRenderer))]
public class BeamScript : MonoBehaviour {


    LineRenderer lineRenderer;    
    RaycastHit hitPoint;

    public float lifeTime;

    public float speed;
    public float damage;
    
    public GameObject Explosion;

    public GameObject From;
    public GameObject Target;
    
    float initialBeamOffset;        // Initial UV offset 
    
    float beamLength;               // Current beam length
    public float beamScale = 1;

    public float UVTime = -6f;            // UV Animation speed

    Vector3 targetPosition;

    // Use this for initialization
    void Start () {
        lineRenderer = GetComponent<LineRenderer>();
        initialBeamOffset = Random.Range(0f, 5f);

        targetPosition = Target.transform.position;
        Raycast();

        Target.SendMessage("doLaserDamage", damage);
    }
	
	// Update is called once per frame
	void Update () {
        lineRenderer.material.SetTextureOffset("_MainTex", new Vector2(Time.time * UVTime + initialBeamOffset, 0f));

        if (Target)
            targetPosition = Target.transform.position;


        Raycast();

        if (lifeTime > 0)
        {
            lifeTime -= Time.deltaTime;
        }
        else
        {
            Destroy(this.gameObject);
        }

    }

    // Hit point calculation
    void Raycast()
    {
        // Prepare structure and create ray
        hitPoint = new RaycastHit();
        //Ray ray = new Ray(From.transform.position, Target.transform.position);
        Ray ray = new Ray(From.transform.position, targetPosition);


        // Calculate default beam proportion multiplier based on default scale and maximum length
        float propMult = 500 * (beamScale / 10f);


            // Get current beam length and update line renderer accordingly
            beamLength = Vector3.Distance(From.transform.position, hitPoint.point);



        //v.Normalize();
        lineRenderer.useWorldSpace = true;
        lineRenderer.SetPosition(0, From.transform.position);
        // lineRenderer.SetPosition(1, Target.transform.position );
        lineRenderer.SetPosition(1, targetPosition);

        // Calculate default beam proportion multiplier based on default scale and current length
        propMult = beamLength * (beamScale / 10f);


        // Adjust impact effect position
        if (Explosion)
        {
            //Explosion.transform.position = Target.transform.position;
            Explosion.transform.position = targetPosition;

        }


        // Set beam scaling according to its length
        lineRenderer.material.SetTextureScale("_MainTex", new Vector2(propMult, 1f));
    }


}
