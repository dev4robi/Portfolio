using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AStarAlgorithm : MonoBehaviour
{
    public AStarNode m_kStartNode;       // 시작점 노드
    public AStarNode m_kDestNode;        // 도착점 노드
    public List<AStarNode> m_lstPath;    // 목적지까지의 경로 노드
    public DirectionBar m_kDirectionBar; // 경로 노드를 그리는 매니저

    private GameObject m_objNodeGrp = null;                           // 노드 오브젝트 그룹
    private List<AStarNode> m_lstOpenNode  = new List<AStarNode>();   // 열린 노드
    private List<AStarNode> m_lstCloseNode = new List<AStarNode>();   // 닫힌 노드

    private float fLastWork = 0.00f;    // 마지막으로 알고리즘을 수행했던 시점

    // Use this for initialization
    void Start ()
    {
        m_kStartNode = null;
        m_kDestNode = null;
        m_objNodeGrp = GameObject.Find("AStarNodeGrp");

        fLastWork = Time.fixedTime;

        if (m_objNodeGrp == null)
        {
            Debug.Log("m_objNodeGrp == null");
        }
    }
	
	// Update is called once per frame
	void Update ()
    {
  		if (fLastWork + 2.00f < Time.fixedTime && Input.GetKeyDown(KeyCode.Alpha3))
        {
            Debug.Log("Start Pathfind Algorithm");
            InitAlgorithm();
            FindAStarPath();
            fLastWork = Time.fixedTime;

            foreach (AStarNode kNode in m_lstPath)
            {
                kNode.SetNodeColor(AStarNode.COLOR_PATH);
            }
        }

        if (Input.GetKeyDown(KeyCode.Alpha0))
        {
            InitAlgorithm();
        }

        if ((Input.GetKey(KeyCode.Alpha1) || Input.GetKey(KeyCode.Alpha2)) && Input.GetKeyDown(KeyCode.Mouse0))
        {
            RaycastHit kHit;
            if (Physics.Raycast(Camera.main.ScreenPointToRay(Input.mousePosition), out kHit) == false) return;

            AStarNode kNode = kHit.transform.gameObject.GetComponent<AStarNode>();
            if (kNode != null)
            {
                if (Input.GetKey(KeyCode.Alpha1))
                {
                    if (m_kStartNode != null)
                    {
                        m_kStartNode.SetNodeColor(AStarNode.COLOR_INIT);
                    }
                    m_kStartNode = kNode;
                    kNode.SetNodeColor(AStarNode.COLOR_START);
                }
                else if (Input.GetKey(KeyCode.Alpha2))
                {
                    if (m_kDestNode != null)
                    {
                        m_kDestNode.SetNodeColor(AStarNode.COLOR_INIT);
                    }
                    m_kDestNode = kNode;
                    kNode.SetNodeColor(AStarNode.COLOR_DEST);
                }
            }
        }
	}

    // 알고리즘 초기화
    private void InitAlgorithm()
    {
        m_kDirectionBar.m_lstPathNodes.Clear();
        m_kDirectionBar.m_bWorking = false;

        m_lstPath.Clear();
        m_lstOpenNode.Clear();
        m_lstCloseNode.Clear();

        AStarNode[] arkNodes = m_objNodeGrp.GetComponentsInChildren<AStarNode>();
        foreach (AStarNode kNode in arkNodes)
        {
            kNode.InitNode();
        }
    }

    // 전달받은 리스트에 전달받은 노드를 추가 (중복시 false 반환, 추가 거부)
    private bool AddNodeToList(List<AStarNode> lstNode, AStarNode kNode)
    {
        if (lstNode.Contains(kNode) == false)
        {
            lstNode.Add(kNode);
            return true;
        }
        else
        {
            return false;
        }
    }

    // 전달받은 리스트에서 전달받은 노드 제거 (존재하지 않을 시 false 반환)
    private bool RemoveNodeFromList(List<AStarNode> lstNode, AStarNode kNode)
    {
        if (lstNode.Contains(kNode) == true)
        {
            lstNode.Remove(kNode);
            return true;
        }
        else
        {
            return false;
        }
    }

    // 현재 시작-도착 노드값으로 A* 길찾기 수행
    public bool FindAStarPath()
    {
        // 1. 시작 노드와 목적지 노드가 존재하는지, 같은 노드인지 확인
        if (m_kStartNode == null)
        {
            Debug.Log("m_kStartNode == null");
            return false;
        }
        if (m_kDestNode == null)
        {
            Debug.Log("m_kDestNode == null");
            return false;
        }
        if (m_kStartNode == m_kDestNode)
        {
            Debug.Log("m_kStartNode == m_kDestNode");
            return false;
        }

        AStarNode kParentNode = m_kStartNode; // 가장 첫번째 부모노드는 시작 노드
        while (true)
        {
            // 1. 부모노드를 열린목록에서 제거하고 닫힌목록에 추가
            RemoveNodeFromList(m_lstOpenNode, kParentNode);
            AddNodeToList(m_lstCloseNode, kParentNode);

            // 2. 부모노드와 연결된 인접노드들을 열린목록에 넣고 비용정보 업데이트
            List<AStarNode> lstLinkedNode = kParentNode.GetLinkedNodeList();
            foreach (AStarNode kNode in lstLinkedNode)
            {
                if (kNode == null)
                {
                    continue;
                }
                else if (kNode == m_kDestNode) // 연결된 노드에 목적지가 포함되어 있다면 목적지에 도달함을 의미
                {
                    kNode.UpdateNodeCost(kParentNode, m_kDestNode);
                    m_kDestNode.ParentNode = kParentNode;
                    m_lstPath.Add(kNode);

                    AStarNode kPathNode = m_kDestNode.ParentNode;
                    while (kPathNode != null)
                    {
                        m_lstPath.Insert(0, kPathNode);
                        kPathNode = kPathNode.ParentNode;
                    }

                    m_kDirectionBar.m_lstPathNodes = m_lstPath;
                    m_kDirectionBar.m_bWorking = true;

                    return true;
                }
                else if (m_lstCloseNode.Contains(kNode) == true) // 닫힌목록에 존재하는 노드는 무시
                {
                    continue;
                }
                else if (AddNodeToList(m_lstOpenNode, kNode) == true) // 열린목록에 존재하지 않던 노드
                {
                    kNode.UpdateNodeCost(kParentNode, m_kDestNode); // 비용 정보 업데이트
                    kNode.ParentNode = kParentNode;                 // 열린 노드에 추가된 인접 노드의 부모 노드를 설정
                }
                else // 열린목록에 존재하던 노드
                {
                    float fNewCostG = kNode.GetUpdatedNodeCostG(kParentNode, m_kDestNode);

                    if (fNewCostG < kNode.G) // 새로운 노드에서 접근하는 G-Cost값이 더 작다면 부모 갱신 및 G-Cost값 변경
                    {
                        kNode.G = fNewCostG;
                        kNode.ParentNode = kParentNode;
                    }
                }
            }

            // 3. 열린목록에 남아있는 노드가 없다면 목적지로 향하는 경로 없음
            if (m_lstOpenNode.Count == 0)
            {
                Debug.Log("Can't found the way to dest.");
                return false;
            }

            // 4. 열린노드의 노드중 가장 작은 F-Cost와 H-Cost를 보유한 노드를 선택
            float fMinCostF = float.MaxValue;
            foreach (AStarNode kNode in m_lstOpenNode)
            {
                if (kNode.F < fMinCostF)
                {
                    fMinCostF = kNode.F;
                }
            }

            List<AStarNode> lstMinF = m_lstOpenNode.FindAll(x => x.F == fMinCostF);
            AStarNode kMinNode = null;
            float fMinCostH = float.MaxValue;
            foreach (AStarNode kNode in lstMinF)
            {
                if (kNode.H < fMinCostH)
                {
                    fMinCostH = kNode.H;
                    kMinNode = kNode;
                }
            }

            if (kMinNode == null)
            {
                Debug.Log("kMinNode == null");
                return false;
            }

            // 5. 선택된 노드를 부모 노드로 만들고 반복
            kParentNode = kMinNode;
        }
    }
}
