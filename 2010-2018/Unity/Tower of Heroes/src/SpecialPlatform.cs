using UnityEngine;
using System.Collections;

public class SpecialPlatform : MonoBehaviour
{

    public enum PlatformType
    {
        PLATFORM_NORMAL, PLATFORM_PIERCE
    }

    public PlatformType enType = PlatformType.PLATFORM_NORMAL;  // 플랫폼 종류

    // Use this for initialization
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {

    }

    void OnTriggerEnter(Collider otherCol)
    {
        if (enType == PlatformType.PLATFORM_PIERCE)
        {
            // 플레이어가 아래서 위로 점프
            if (otherCol.gameObject.tag == "Player")
            {
                BoxCollider[] boxCols = this.gameObject.GetComponents<BoxCollider>();

                for (int i = 0; i < boxCols.Length; ++i)
                {
                    if (boxCols[i].isTrigger == false)
                    {
                        boxCols[i].enabled = false;
                    }
                }
            }
        }
    }

    void OnTriggerExit(Collider otherCol)
    {
        if (enType == PlatformType.PLATFORM_PIERCE)
        {
            // 플레이어가 아래서 위로 점프
            if (otherCol.gameObject.tag == "Player")
            {
                BoxCollider[] boxCols = this.gameObject.GetComponents<BoxCollider>();

                for (int i = 0; i < boxCols.Length; ++i)
                {
                    if (boxCols[i].isTrigger == false)
                    {
                        boxCols[i].enabled = true;
                    }
                }
            }
        }
    }
}
