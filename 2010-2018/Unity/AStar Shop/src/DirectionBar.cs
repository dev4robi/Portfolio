using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DirectionBar : MonoBehaviour
{
    public List<AStarNode> m_lstPathNodes;  // 길찾기 정보가 순서대로 담긴 노드 리스트
    public bool m_bWorking = false;         // 동작중 여부

    static float FLOAT_EXPEND_SPD = 30.0f;

	// Use this for initialization
	void Start ()
    {
		
	}
	
	// Update is called once per frame
	void Update ()
    {
        if (m_bWorking == false) return;
        if (m_lstPathNodes.Count < 2) // 경로 안내 작업 완료
        {
            m_lstPathNodes.Clear();
            m_bWorking = false;
            return;
        }

        // 경로 안내 작업 수행
        AStarNode kNodeBegin = m_lstPathNodes[0];
        AStarNode kNodeEnd = m_lstPathNodes[1];
        Transform transBegin = kNodeBegin.m_objDirectionBar.transform;

        float fDist = Vector3.Distance(kNodeEnd.transform.position, kNodeBegin.transform.position);
        float fBarLen = transBegin.localScale.z;
        float fBarExtendAmount = FLOAT_EXPEND_SPD * Time.deltaTime;

        fBarLen += fBarExtendAmount;

        if (fBarLen >= fDist) // 목표 노드에 바가 도달
        {
            fBarLen = fDist;
            m_lstPathNodes.RemoveAt(0);
        }

        Vector3 vecLocalScale = transBegin.localScale;
        vecLocalScale.z = fBarLen;
        transBegin.localScale = vecLocalScale;
        transBegin.LookAt(kNodeEnd.transform);
    }
}
