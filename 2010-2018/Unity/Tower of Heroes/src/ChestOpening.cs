using UnityEngine;
using System.Collections;

public class ChestOpening : MonoBehaviour {
    public enum DropObject { DROP_GOLD, DROP_RUNE, DROP_BOTH_RANDOM };
    public DropObject enDrop = DropObject.DROP_GOLD;    // 기본적으로 주는 아이템은 골드
    public int iGoldMin = 0;                            // 드롭되는 최소 골드
    public int iGoldMax = 0;                            // 드롭되는 최대 골드
    public int iRuneMin = 0;                            // 드롭되는 최소 룬
    public int iRuneMax = 0;                            // 드롭되는 최대 룬

    private bool bIsOpened = false;                     // 상자 열린 여부
    private GameObject objChestCover = null;            // 상자 뚜껑 오브젝트

	// Use this for initialization
	void Start () {
        // 상자 뚜껑을 핸들링
        try
        {
            objChestCover = this.transform.FindChild("cover").gameObject;
        }
        catch (System.Exception e) { };
    }
	
	// Update is called once per frame
	void Update () {
	
	}

    // 상자를 여는 메소드
    public void OpenChest()
    {
        // 상자가 닫힌 상태이면
        if (bIsOpened == false)
        {
            // 상자를 열린 상태로 변경
            bIsOpened = true;

            // 뚜껑에 물리 효과를 주어 날려버리고 중력을 사용하도록 함
            Rigidbody rigid = objChestCover.GetComponent<Rigidbody>();
            rigid.useGravity = true;
            rigid.isKinematic = false;
            rigid.AddForce(new Vector3(0.0f, 100.0f, -200.0f));

            // 보상 종류에 따른 선처리
            float fRandomAmount = 0.0f;                                                                 // 획득될 자원의 양
            string resPathStr = "Prefabs/Items/Res_GoldCoin";                                           // 프리펩 경로
            ItemResource.ItemType enResType = ItemResource.ItemType.ITEM_GOLD;                          // 아이템 종류
            GameObject bonusGrp = GameObject.Find("Stage").transform.FindChild("Bonus").gameObject;     // 아이템이 보관될 그룹 오브젝트

            // 드롭되는 아이템(자원) 종류 설정
            if (enDrop == DropObject.DROP_GOLD)
            {
                enResType = ItemResource.ItemType.ITEM_GOLD;
            }
            else if (enDrop == DropObject.DROP_RUNE)
            {
                enResType = ItemResource.ItemType.ITEM_RUNE;
            }
            else if (enDrop == DropObject.DROP_BOTH_RANDOM)
            {
                // 50% 이상의 확률로 금화 생성
                if (Random.Range(0.0f, 1.0f) >= 0.5f)
                {
                    enResType = ItemResource.ItemType.ITEM_GOLD;
                }
                else
                {
                    enResType = ItemResource.ItemType.ITEM_RUNE;
                }
            }

            // 종류에 따른 경로명과 범위내 무작위 획득량 지정
            if (enResType == ItemResource.ItemType.ITEM_GOLD)
            {
                resPathStr = "Prefabs/Items/Res_GoldCoin";
                fRandomAmount = Random.Range(iGoldMin, iGoldMax);
            }
            else if (enResType == ItemResource.ItemType.ITEM_RUNE)
            {
                resPathStr = "Prefabs/Items/Res_RuneStone";
                fRandomAmount = Random.Range(iRuneMin, iRuneMax);
            }

            // 보상 생성 및 보상 속성 설정
            GameObject resItem = Object.Instantiate(Resources.Load(resPathStr) as Object, this.transform.position, Quaternion.identity) as GameObject;
            resItem.transform.parent = bonusGrp.transform;

            ItemResource resVar = resItem.GetComponent<ItemResource>();
            resVar.SetValueOfItem(enResType, (int)fRandomAmount, 0);
        }
    }
}
