using UnityEngine;
using System.Collections;

public class MissileScript : MonoBehaviour {

    public GameObject Target;
    public GameObject Explosion;


    public float lifeTime;

    public float velocity;
    public float alignSpeed;

    public float damage;



    ParticleSystem[] particles;
    MeshRenderer meshRenderer;

    new Transform transform;                    // Cached transform
    bool isHit = false;

    Vector3 targetLastPos;
    Vector3 step;

    float flightTime;

    // Use this for initialization
    void Start () {
        transform = GetComponent<Transform>();
        particles = GetComponentsInChildren<ParticleSystem>();

        meshRenderer = GetComponent<MeshRenderer>();

        flightTime = lifeTime;
    }
	

    void FixedUpdate()
    {
        if(flightTime > 0)
        {
            flightTime -= Time.deltaTime;
        }
        else
        {
            Destroy(this.gameObject);

            if (Explosion)
            {
                GameObject obj = Instantiate(Explosion, transform.position, Quaternion.identity) as GameObject;
                obj.transform.parent = null;
                Destroy(this.gameObject);
            }
        }
    }

	// Update is called once per frame
	void Update () {

        if (Target)
        {
            Vector3 hitPos = F3DPredictTrajectory.Predict(transform.position, Target.transform.position, targetLastPos, velocity);
            targetLastPos = Target.transform.position;

            transform.rotation = Quaternion.Lerp(transform.rotation, Quaternion.LookRotation(hitPos - transform.position), Time.deltaTime * alignSpeed);
        }
        step = transform.forward * Time.deltaTime * velocity;

        transform.position += step;
    }

    void OnTriggerEnter(Collider other)
    {

        if (other.gameObject == Target)
        {
            if (Explosion)
            {
                GameObject obj = Instantiate(Explosion, transform.position, Quaternion.identity) as GameObject;
                obj.transform.parent = null;
                Destroy(this.gameObject);
            }

            if (Target != null) Target.SendMessage("doMissileDamage", damage);
        }
    }
}
