using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AStarNode : MonoBehaviour
{
    public List<AStarNode> m_lstLinkedNode; // 연결된 노드
    public AStarNode m_kParentNode;         // 부모 노드
    public GameObject m_objDirectionBar;    // 방향을 시각적으로 표시해줄 바

    private float m_G = 0.0f; // G (시작 노드로부터 거쳐온 거리)
    private float m_H = 0.0f; // H (목적지 노드와의 거리)
    private float m_F = 0.0f; // G + H

    public static Color COLOR_INIT  = new Color(1.0f, 1.0f, 1.0f);  // 흰색
    public static Color COLOR_PATH  = new Color(1.0f, 0.0f, 0.0f);  // 적색
    public static Color COLOR_START = new Color(0.0f, 1.0f, 0.0f);  // 녹색
    public static Color COLOR_DEST  = new Color(0.0f, 0.0f, 1.0f);  // 청색

    public AStarNode ParentNode
    {
        set { m_kParentNode = value; }
        get { return m_kParentNode; }
    }
    public float G
    {
        set { m_G = value; }
        get { return m_G; }
    }
    public float H
    {
        set { m_H = value; }
        get { return m_H; }
    }
    public float F
    {
        set { m_F = value; }
        get { return m_F; }
    }

    // Use this for initialization
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {

    }

    // 해당 노드에서 연결된 모든 노드 반환
    public List<AStarNode> GetLinkedNodeList()
    {
        return m_lstLinkedNode;
    }

    // 입력받은 노드로부터의 거리 반환
    public float DistFromNode(AStarNode node)
    {
        return Vector3.Distance(node.transform.position, transform.position);
    }

    // 노드 초기화
    public void InitNode()
    {
        InitNodeCost();
        InitNodeDirection();
        SetNodeColor(COLOR_INIT);
        ParentNode = null;
    }

    // 노드 방향정보 초기화
    public void InitNodeDirection()
    {
        transform.rotation = Quaternion.identity;
        Vector3 vecLocalScale = m_objDirectionBar.transform.localScale;
        vecLocalScale.z = 0.0f;
        m_objDirectionBar.transform.localScale = vecLocalScale;
    }

    // 노드 Cost정보 초기화
    public void InitNodeCost()
    {
        G = 0.0f;
        H = 0.0f;
        F = 0.0f;
    }

    // 노드 색상 적용
    public void SetNodeColor(Color kColor)
    {
        GetComponent<MeshRenderer>().material.color = kColor;
    }

    // 노드의 G-Cost, H-Cost, F-Cost를 연산하여 적용
    public void UpdateNodeCost(AStarNode lastNode, AStarNode destNode)
    {
        G = DistFromNode(lastNode) + lastNode.G;
        H = DistFromNode(destNode);
        F = G + H;
    }

    // 업데이트후의 F-Cost값을 반환 (갱신하지 않음)
    public float GetUpdatedNodeCostG(AStarNode lastNode, AStarNode destNode)
    {
        float fG = DistFromNode(lastNode) + lastNode.G;
        return fG;
    }
}
