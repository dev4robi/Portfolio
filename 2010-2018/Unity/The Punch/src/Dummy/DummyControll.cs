using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DummyControll : MonoBehaviour
{
    private int iLastPlayedAnimIndex;

    public Animator animator; // Dummy's humanoid animator
    public int[] hitCounter; // Hit counter divided by HitTarget parts
    public float[] hitPowers; // Hit power divided by HitTarget parts
    public float fFinisherPower; // Finisher hit's power;
    public Light lightComponent;
    public AudioSource audioSource;
    public AudioClip[] hitSounds;
    public AudioClip[] deathSounds;

    private void Start()
    {
        // Initialize all
        if (!Initialize()) Debug.Log("DummyControll Initialization has error!");
    }

    private void Update()
    {

    }

    #region # INITIALIZATION #
    public bool Initialize()
    {
        bool bResult = true;

        iLastPlayedAnimIndex = 0;

        if (lightComponent == null) lightComponent = transform.FindChild("Point Light").GetComponent<Light>();
        if (audioSource == null) audioSource = GetComponent<AudioSource>();

        bResult &= FindAnimator();

        bResult &= InitHitCount();

        return bResult;
    }
    #endregion

    #region # HIT_COUNTER #
    public bool InitHitCount()
    {
        hitCounter = new int[(int)GameDatas.HitTarget.HT_LOWER_LEFT + 1];
        hitPowers = new float[hitCounter.Length];

        for (int i = 0; i < hitCounter.Length; ++i) hitCounter[i] = 0;

        if (hitCounter != null)
            return true;
        else
            return false;
    }

    public void IncreaseHitCount(GameDatas.HitTarget target)
    {
        int iIndex = (int)target;

        ++hitCounter[iIndex];
    }

    public int GetHitCount(GameDatas.HitTarget target)
    {
        int iIndex = (int)target;

        return hitCounter[iIndex];
    }
    #endregion

    #region # ANIMATION_MANAGEMENT #
    public bool FindAnimator()
    {
        animator = GetComponent<Animator>();

        if (animator != null)
            return true;
        else
            return false;
    }

    public void PlayAnimation(string trigName, int index)
    {
        iLastPlayedAnimIndex = index;
        animator.SetInteger(GameDatas.STR_RANDOMINDEX_NAME, index);
        animator.SetTrigger(trigName);
    }

    public void PlayRandomDemoAnim()
    {
        int iIndex;

        do
        {
            iIndex = Random.Range(0, GameDatas.INT_DEMO_ANIM_COUNT);
        } while (iIndex == iLastPlayedAnimIndex); // For evade last played animation index number

        PlayAnimation(GameDatas.STR_DEMO_TRIG_NAME, iIndex);
    }
    #endregion

    #region # HIT_ACTION #
    public void HitByNormal(GameDatas.HitTarget target, float power)
    {
        int iAnimIndex;
        string strTrigName;
        GameDatas.HitTarget enTarget;

        switch (target)
        {
            default:
            case GameDatas.HitTarget.HT_HEAD:
                iAnimIndex = Random.Range(0, GameDatas.INT_HEADHIT_ANIM_COUNT);
                strTrigName = GameDatas.STR_HEADHIT_TRIG_NAME;
                enTarget = GameDatas.HitTarget.HT_HEAD;
                hitPowers[0] += power;
                break;
            case GameDatas.HitTarget.HT_UPPER_RIGHT:
                iAnimIndex = Random.Range(0, GameDatas.INT_UPPERHIT_RIGHT_ANIM_COUNT);
                strTrigName = GameDatas.STR_UPPERHIT_RIGHT_TRIG_NAME;
                enTarget = GameDatas.HitTarget.HT_UPPER_RIGHT;
                hitPowers[1] += power;
                break;
            case GameDatas.HitTarget.HT_UPPER_LEFT:
                iAnimIndex = Random.Range(0, GameDatas.INT_UPPERHIT_LEFT_ANIM_COUNT);
                strTrigName = GameDatas.STR_UPPERHIT_LEFT_TRIG_NAME;
                enTarget = GameDatas.HitTarget.HT_UPPER_LEFT;
                hitPowers[2] += power;
                break;
            case GameDatas.HitTarget.HT_LOWER_RIGHT:
                iAnimIndex = Random.Range(0, GameDatas.INT_LOWERHIT_RIGHT_ANIM_COUNT);
                strTrigName = GameDatas.STR_LOWERHIT_RIGHT_TRIG_NAME;
                enTarget = GameDatas.HitTarget.HT_LOWER_RIGHT;
                hitPowers[3] += power;
                break;
            case GameDatas.HitTarget.HT_LOWER_LEFT:
                iAnimIndex = Random.Range(0, GameDatas.INT_LOWERHIT_LEFT_ANIM_COUNT);
                strTrigName = GameDatas.STR_LOWERHIT_LEFT_TRIG_NAME;
                enTarget = GameDatas.HitTarget.HT_LOWER_LEFT;
                hitPowers[4] += power;
                break;
        }

        if (!audioSource.isPlaying) audioSource.PlayOneShot(hitSounds[Random.Range(0, hitSounds.Length)]);
        GameDatas.OBJ_MAINSCENE.CreateSFX(GameDatas.OBJ_MAINSCENE.punchSFX[Random.Range(0, GameDatas.OBJ_MAINSCENE.punchSFX.Length)]);
        IncreaseHitCount(enTarget);
        PlayAnimation(strTrigName, iAnimIndex);
    }

    public void HitByFinisher(GameDatas.HitTarget target, float power)
    {
        int iAnimIndex;
        string strTrigName = GameDatas.STR_FINISHERHIT_TRIG_NAME;

        if (power >= GameDatas.FLOAT_FINISHER_LEVELS[0])
        {
            iAnimIndex = 0;

            Vector3 powerDir;

            switch (target)
            {
                default:
                case GameDatas.HitTarget.HT_HEAD:
                    powerDir = Vector3.forward + Vector3.up * Random.Range(0.35f, 0.5f);
                    break;
                case GameDatas.HitTarget.HT_UPPER_LEFT:
                case GameDatas.HitTarget.HT_LOWER_LEFT:
                    powerDir = Vector3.forward + Vector3.up * 0.25f + Vector3.left * Random.Range(0.15f, 0.25f);
                    break;
                case GameDatas.HitTarget.HT_UPPER_RIGHT:
                case GameDatas.HitTarget.HT_LOWER_RIGHT:
                    powerDir = Vector3.forward + Vector3.up * 0.25f + Vector3.right * Random.Range(0.15f, 0.25f);
                    break;
            }

            Rigidbody rigid = GetComponent<Rigidbody>();
            rigid.isKinematic = false;
            rigid.AddForce(powerDir * Random.Range(40.0f, 50.0f), ForceMode.Impulse);
            lightComponent.enabled = true;

            GameDatas.OBJ_MAINSCENE.CreateSFX(deathSounds[Random.Range(0, deathSounds.Length)]);
            GameDatas.OBJ_MAINSCENE.CreateSFX(GameDatas.OBJ_MAINSCENE.punchSFX[Random.Range(0, GameDatas.OBJ_MAINSCENE.punchSFX.Length)]);
        }
        else if (power >= GameDatas.FLOAT_FINISHER_LEVELS[1])
        {
            iAnimIndex = 1;
        }
        else if (power >= GameDatas.FLOAT_FINISHER_LEVELS[2])
        {
            iAnimIndex = 2;
        }
        else if (power >= GameDatas.FLOAT_FINISHER_LEVELS[3])
        {
            iAnimIndex = 3;
        }
        else
        {
            iAnimIndex = 4;
        }

        IncreaseHitCount(target);
        PlayAnimation(strTrigName, iAnimIndex);
        fFinisherPower = power * (GameDatas.INT_MAX_SCORE - GameDatas.INT_MIN_SCORE) + GameDatas.INT_MIN_SCORE;
    }

    #endregion
}

