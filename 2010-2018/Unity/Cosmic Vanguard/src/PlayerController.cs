using UnityEngine;
using System;
using System.Collections;

public class PlayerController : MonoBehaviour {

    public enum WeaponType
    {
        Laser, Missile
    }

    public enum SkillName
    {
        PrimaryDamage, PrimaryAttackRate,
        SecondaryDamage, SecondaryAttackRage,
        MoveSpeed,
        DamageNova, SlowNova, 
        LaserArmament,
        EMP,
        Napalm

    }


    public enum SkillTargetType
    {
        Self, Nova, Projectile
    }



    [Serializable]
    public class MoveParam
    {
        public float MaxSpeed;
        public float Acceleration;
        public float rotationSpeed;
    }

    [Serializable]
    public class PrimaryWeaponParam
    {
        public GameObject[] ShootingPoint;
        public GameObject Muzzle;
        public GameObject Bullet;
        public GameObject Explosion;



        public int level;

        public float[] shootingInterval;

        public float[] bulletDamage;
        public float bulletSpeed;
    }

    [Serializable]
    public class SecondaryWeaponParam
    {
        public WeaponType weaponType;
        public GameObject[] ShootingPoint;
        public GameObject weaponObject;



        public int level;

        public float lifeTime;
        public float[] shootingInterval;
        public float[] damage;

        public float missile_velocity;
        public float missile_alignSpeed;
        public float missile_lifeTime;
    }

    [Serializable]
    public class SkillParam
    {

        public SkillTargetType skillTargetType;
        public GameObject SkillEffect;
        public GameObject SkillObject;

        public float coolDown;
        public float effectLifetime;

        public float duration;

        public float affect_speed;
        public float affect_primary;
        public float affect_secondary;
        public float affect_primaryRate;
        public float affect_secondaryRate;

        public float damage;
        public float shieldDamage;

        public float slow_multiplier;

    }


    public MoveParam moveParam;
    public PrimaryWeaponParam primaryParam;
    public SecondaryWeaponParam secondaryParam;
    public SkillParam skillParam;

    public GameObject target;
    public GameObject secondaryTarget;

    Rigidbody rigidbody;
    ParticleSystem[] thrusters;

    float primaryInterval = 0;
    float secondaryInterval = 0;
    float skillInterval = 0;


    float affect_duration;
    float additional_speed;
    float additional_primaryDamage;
    float additional_primaryRate;


    float additional_secondaryDamage;
    float additional_secondaryRate;


    void Start () {
        rigidbody = GetComponent<Rigidbody>();
        thrusters = GetComponentsInChildren<ParticleSystem>();
	}
	
    void FixedUpdate()
    {
        if (secondaryInterval >= 0)
            secondaryInterval -= Time.deltaTime;

        if (skillInterval >= 0)
            skillInterval -= Time.deltaTime;

        MovementControl();
        PrimaryWeaponControl();

        if(affect_duration >= 0)
        {
            affect_duration -= Time.deltaTime;
        }
        else
        {
            additional_primaryDamage = 0;
            additional_secondaryDamage = 0;
            additional_speed = 0;
            additional_secondaryRate = 0;
        }
    }


    void Update () {
        

        if (Input.GetKeyDown(KeyCode.Space ) )
        {

            if (skillInterval < 0)
            {
                    SkillControl();
                skillInterval = skillParam.coolDown;
            }
        }
    }

    private void MovementControl()
    {
        float moveHorizontal = Input.GetAxis("Horizontal");
        float moveVertical = Input.GetAxis("Vertical");

        bool isLeftDown = Input.GetKey(KeyCode.A);
        bool isRightDown = Input.GetKey(KeyCode.D);
        bool isTopDown = Input.GetKey(KeyCode.W);
        bool isBottomDown = Input.GetKey(KeyCode.S);

        Vector3 movement = new Vector3(moveHorizontal, 0.0f, moveVertical);

        if (isLeftDown | isRightDown | isTopDown | isBottomDown)
        {
            rigidbody.velocity = Vector3.Lerp(rigidbody.velocity, movement * (moveParam.MaxSpeed + additional_speed), Time.deltaTime * moveParam.Acceleration);
            for (int i = 0; i < thrusters.Length; i++)
            {
                ParticleSystem p = thrusters[i];
                p.enableEmission = true;
            }
        }
        else
        {
            rigidbody.velocity = Vector3.Lerp(rigidbody.velocity, Vector3.zero, Time.deltaTime * moveParam.Acceleration * 2);
            for (int i = 0; i < thrusters.Length; i++)
            {
                ParticleSystem p = thrusters[i];
                p.enableEmission = false;
            }
        }



        if(isRightDown & isTopDown)
            transform.rotation = Quaternion.Slerp(transform.rotation, Quaternion.Euler(0, 225, 0), Time.deltaTime * moveParam.rotationSpeed);
        else if (isRightDown & isBottomDown)
            transform.rotation = Quaternion.Slerp(transform.rotation, Quaternion.Euler(0, 315, 0), Time.deltaTime * moveParam.rotationSpeed);
        else if (isLeftDown & isTopDown)
            transform.rotation = Quaternion.Slerp(transform.rotation, Quaternion.Euler(0, 135, 0), Time.deltaTime * moveParam.rotationSpeed);
        else if (isLeftDown & isBottomDown)
            transform.rotation = Quaternion.Slerp(transform.rotation, Quaternion.Euler(0, 45, 0), Time.deltaTime * moveParam.rotationSpeed);

        else if(isRightDown)
            transform.rotation = Quaternion.Slerp(transform.rotation, Quaternion.Euler(0, 270, 0), Time.deltaTime * moveParam.rotationSpeed);
        else if (isLeftDown)
            transform.rotation = Quaternion.Slerp(transform.rotation, Quaternion.Euler(0, 90, 0), Time.deltaTime * moveParam.rotationSpeed);

        else if(isTopDown)
            transform.rotation = Quaternion.Slerp(transform.rotation, Quaternion.Euler(0, 180, 0), Time.deltaTime * moveParam.rotationSpeed);
        else if (isBottomDown)
            transform.rotation = Quaternion.Slerp(transform.rotation, Quaternion.Euler(0, 0, 0), Time.deltaTime * moveParam.rotationSpeed);

    }

    private void PrimaryWeaponControl()
    {
        if(target != null)
        {
            if(primaryInterval < 0)
            {
                for(int i = 0; i < primaryParam.ShootingPoint.Length; i++)
                {
                    Instantiate(primaryParam.Muzzle, primaryParam.ShootingPoint[i].transform.position, Quaternion.identity);
                    GameObject bullet = Instantiate(primaryParam.Bullet, primaryParam.ShootingPoint[i].transform.position, Quaternion.identity) as GameObject;
                    BulletScript script = bullet.GetComponent<BulletScript>();
                    script.Target = target;
                    script.speed = primaryParam.bulletSpeed;
                    script.damage = primaryParam.bulletDamage[primaryParam.level - 1] + additional_primaryDamage;
                    script.Explosion = primaryParam.Explosion;
                }
                primaryInterval = primaryParam.shootingInterval[primaryParam.level - 1] - additional_primaryRate;
            }
            else
            {
                primaryInterval -= Time.deltaTime;
            }
        }
    }

    private void SecondaryWeaponControl()
    {
        if (secondaryTarget != null)
        {

            for (int i = 0; i < secondaryParam.ShootingPoint.Length; i++)
            {
                if (secondaryParam.weaponType == WeaponType.Laser)
                {

                    GameObject muzzle = Instantiate(secondaryParam.weaponObject, secondaryParam.ShootingPoint[i].transform.position, Quaternion.identity) as GameObject;
                    BeamScript script = muzzle.GetComponent<BeamScript>();
                    muzzle.transform.parent = this.transform;
                    script.Target = secondaryTarget;
                    script.From = secondaryParam.ShootingPoint[i];
                    script.lifeTime = secondaryParam.lifeTime;
                    script.damage = secondaryParam.damage[secondaryParam.level -1] + additional_secondaryDamage;
                }
                else if (secondaryParam.weaponType == WeaponType.Missile)
                {

                    GameObject lunch = Instantiate(secondaryParam.weaponObject, secondaryParam.ShootingPoint[i].transform.position, Quaternion.identity) as GameObject;
                    MissileScript script = lunch.GetComponent<MissileScript>();
                    script.Target = secondaryTarget;
                    script.velocity = secondaryParam.missile_velocity;
                    script.alignSpeed = secondaryParam.missile_alignSpeed;
                    script.damage = secondaryParam.damage[secondaryParam.level - 1] + additional_secondaryDamage;
                    script.lifeTime = secondaryParam.missile_lifeTime;
                }
            }

            secondaryTarget = null;
            secondaryInterval = secondaryParam.shootingInterval[secondaryParam.level - 1] - additional_secondaryRate;

        }
    }

    void SkillControl()
    {
        if(skillParam.skillTargetType == SkillTargetType.Self)
        {
            if(skillParam.SkillEffect)
            { 
                GameObject selfEffect = Instantiate(skillParam.SkillEffect, transform.position, Quaternion.identity) as GameObject;
                selfEffect.transform.parent = transform;
                Destroy(selfEffect, skillParam.duration);
            }
            affect_duration = skillParam.duration;
            additional_primaryDamage = skillParam.affect_primary;
            additional_primaryRate = skillParam.affect_primaryRate;
            additional_secondaryDamage = skillParam.affect_secondary;
            additional_speed = skillParam.affect_speed;
            additional_secondaryRate = skillParam.affect_secondaryRate;
        }
        else if (skillParam.skillTargetType == SkillTargetType.Nova)
        {
            if (skillParam.SkillEffect)
            {
                Debug.Log("Nova");

                GameObject novaEffect = Instantiate(skillParam.SkillEffect, transform.position, Quaternion.identity) as GameObject;
                novaEffect.transform.parent = transform;
                NovaScript script = novaEffect.GetComponent<NovaScript>();
                script.lifeTime = skillParam.effectLifetime;

                script.damage = skillParam.damage;
                script.affectSpeed = skillParam.slow_multiplier;
                script.affectDuration = skillParam.duration;
                

                Destroy(novaEffect, skillParam.duration);
           
            }
        }

        secondaryTarget = null;
    }

    void OnTriggerEnter(Collider other)
    {
        if(other.tag == "Enemy")
        {
            target = other.gameObject;
        }
    }

    void OnTriggerExit(Collider other)
    {
        if (other.gameObject == target)
        {
            target = null;

        }
    }

    void secondaryAttack(GameObject obj)
    {
        if (secondaryInterval > 0)
        {

        }
        else
        {
            secondaryTarget = obj;
            SecondaryWeaponControl();
        }
    }

    void doUpgrade()
    {
        primaryParam.level += 1;
        if(primaryParam.level >= 5)
        {
            primaryParam.level= 4;
        }

        secondaryParam.level += 1;

        if (secondaryParam.level >= 5)
        {
            secondaryParam.level = 4;
        }

    }
}
