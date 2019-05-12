using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TestCase : MonoBehaviour
{
    const int INT_WIDTH = 4;
    const int INT_HEIGHT = 4;
    const float FLOAT_DIST_WIDTH = 5.0f;
    const float FLOAT_DIST_HEIGHT = 5.0f;

    public GameObject m_NodeObject;

	// Use this for initialization
	void Start ()
    {
        /*
        GameObject AStarNodeGrp = GameObject.Find("AStarNodeGrp");
        GameObject[,] arrkAStarNode = new GameObject[INT_HEIGHT, INT_WIDTH];

        // 노드 오브젝트 생성
		for (int i = 0; i < INT_HEIGHT; ++i)
        {
            for (int j = 0; j < INT_WIDTH; ++j)
            {
                arrkAStarNode[i, j] = Instantiate(m_NodeObject, new Vector3(j * FLOAT_DIST_WIDTH, 0.0f, i * FLOAT_DIST_HEIGHT), Quaternion.identity, AStarNodeGrp.transform);
                arrkAStarNode[i, j].name = arrkAStarNode[i, j].name + string.Format("[{0}, {1}]", i, j);
            }
        }

        // 노드 오브젝트 링크정보 생성
        for (int i = 0; i < INT_HEIGHT; ++i)
        {
            for (int j = 0; j < INT_WIDTH; ++j)
            {
                AStarNode kNode = arrkAStarNode[i, j].GetComponent<AStarNode>();

                if (kNode == null)
                {
                    Debug.Log("AStarNode Script : kNode == null " + string.Format("[{0}, {1}]", i, j));
                    continue;
                }

                List<AStarNode> lstSurroundingNode = new List<AStarNode>();
                int im1 = i - 1, ip1 = i + 1;
                int jm1 = j - 1, jp1 = j + 1;

                if (im1 > -1) lstSurroundingNode.Add(arrkAStarNode[im1, j].GetComponent<AStarNode>());
                if (ip1 < INT_HEIGHT) lstSurroundingNode.Add(arrkAStarNode[ip1, j].GetComponent<AStarNode>());
                if (jm1 > -1) lstSurroundingNode.Add(arrkAStarNode[i, jm1].GetComponent<AStarNode>());
                if (jp1 < INT_WIDTH) lstSurroundingNode.Add(arrkAStarNode[i, jp1].GetComponent<AStarNode>());

                kNode.m_lstLinkedNode = lstSurroundingNode;
            }
        }
        */
	}
	
	// Update is called once per frame
	void Update ()
    {
		
	}
}
