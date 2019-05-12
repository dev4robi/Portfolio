using UnityEngine;
using System.Collections;

public class BulletScript : MonoBehaviour {

    public float lifeTime;

    public float speed;
    public float damage;

    public GameObject Explosion;

    public GameObject Target;

    // Use this for initialization
    void Start () {


    }

    // Update is called once per frame
    void FixedUpdate () {
        if (Target)
        {
            transform.rotation = Quaternion.FromToRotation(transform.position, Target.transform.position);
            transform.position = Vector3.MoveTowards(transform.position, Target.transform.position, Time.deltaTime * speed);
        }
        else
        {
            if (Explosion)
            {
                GameObject obj = Instantiate(Explosion, transform.position, Quaternion.identity) as GameObject;
                obj.transform.parent = null;
                Destroy(this.gameObject);
            }

        }
    }

    void Update()
    {

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

            Target.SendMessage("doDamage", damage);
        }
    }
}
