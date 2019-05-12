using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CamController : MonoBehaviour
{
    public float m_fXpos, m_fZpos;  // 카메라 중점의 X, Z좌표
    public float m_fDistFromPos;    // 카메라 중점과 카메라 사이의 거리
    public float m_fRotateAngle;    // 카메라 중점의 회전각

    public float m_fCamMoveSpd = 40.0f;     // 카메라 이동속도
    public float m_fCamZoomSpd = 30.0f;     // 카메라 줌 속도
    public float m_fCamRotateSpd = 120.0f;  // 카메라 회전 속도

    const float FLOAT_MIN_XPOS = -30.0f;
    const float FLOAT_MAX_XPOS = 30.0f;
    const float FLOAT_MIN_ZPOS = -20.0f;
    const float FLOAT_MAX_ZPOS = 20.0f;
    const float FLOAT_MAX_ZOOMOUT = 40.0f;
    const float FLOAT_MIN_ZOOMIN = 10.0f;

	// Use this for initialization
	void Start ()
    {
    }
	
	// Update is called once per frame
	void Update ()
    {
		if (Input.GetKey(KeyCode.W)) MoveFoward();
        if (Input.GetKey(KeyCode.S)) MoveBackward();
        if (Input.GetKey(KeyCode.A)) MoveLeftside();
        if (Input.GetKey(KeyCode.D)) MoveRightside();
        if (Input.GetKey(KeyCode.Q)) ZoomIn();
        if (Input.GetKey(KeyCode.E)) ZoomOut();
        if (Input.GetKey(KeyCode.Z)) RotateClockwise();
        if (Input.GetKey(KeyCode.C)) RotateCounterClockwise();
	}

    // 카메라 앞으로 전진이동
    public void MoveFoward()
    {
        Vector3 vecMoveDir = transform.forward;
        float fMoveDist = m_fCamMoveSpd * Time.deltaTime;
        transform.position += vecMoveDir * fMoveDist;
        CorrectXZPosBoundary();
    }

    // 카메라 뒤로 후퇴이동
    public void MoveBackward()
    {
        Vector3 vecMoveDir = transform.forward;
        float fMoveDist = m_fCamMoveSpd * Time.deltaTime;
        transform.position -= vecMoveDir * fMoveDist;
        CorrectXZPosBoundary();
    }

    // 카메라 좌측으로 이동
    public void MoveLeftside()
    {
        Vector3 vecMoveDir = transform.right;
        float fMoveDist = m_fCamMoveSpd * Time.deltaTime;
        transform.position -= vecMoveDir * fMoveDist;
        CorrectXZPosBoundary();
    }

    // 카메라 우측으로 이동
    public void MoveRightside()
    {
        Vector3 vecMoveDir = transform.right;
        float fMoveDist = m_fCamMoveSpd * Time.deltaTime;
        transform.position += vecMoveDir * fMoveDist;
        CorrectXZPosBoundary();
    }

    // 카메라 +X방향으로 이동
    public void MovePlusX()
    {
        float fMoveX = m_fCamMoveSpd * Time.deltaTime;

        m_fXpos += fMoveX;

        if (m_fXpos > FLOAT_MAX_XPOS)
        {
            m_fXpos = FLOAT_MAX_XPOS;
        }

        Vector3 vecPos = transform.position;
        vecPos.x = m_fXpos;
        transform.position = vecPos;
    }

    // 카메라 -X방향으로 이동
    public void MoveMinusX()
    {
        float fMoveX = m_fCamMoveSpd * Time.deltaTime;

        m_fXpos -= fMoveX;

        if (m_fXpos < FLOAT_MIN_XPOS)
        {
            m_fXpos = FLOAT_MIN_XPOS;
        }

        Vector3 vecPos = transform.position;
        vecPos.x = m_fXpos;
        transform.position = vecPos;
    }

    // 카메라 +Z방향으로 이동
    public void MovePlusZ()
    {
        float fMoveZ = m_fCamMoveSpd * Time.deltaTime;

        m_fZpos += fMoveZ;

        if (m_fZpos > FLOAT_MAX_ZPOS)
        {
            m_fZpos = FLOAT_MAX_ZPOS;
        }

        Vector3 vecPos = transform.position;
        vecPos.z = m_fZpos;
        transform.position = vecPos;
    }

    // 카메라 -Z방향으로 이동
    public void MoveMinusZ()
    {
        float fMoveZ = m_fCamMoveSpd * Time.deltaTime;

        m_fZpos -= fMoveZ;

        if (m_fZpos < FLOAT_MIN_ZPOS)
        {
            m_fZpos = FLOAT_MIN_ZPOS;
        }

        Vector3 vecPos = transform.position;
        vecPos.z = m_fZpos;
        transform.position = vecPos;
    }

    // 카메라 줌인
    public void ZoomIn()
    {
        Vector3 vecDir = Camera.main.transform.forward;
        float fZoomDist = m_fCamZoomSpd * Time.deltaTime;

        Vector3 vecLastPos = Camera.main.transform.position;
        Camera.main.transform.position += vecDir * fZoomDist;

        if (Vector3.Distance(Camera.main.transform.position, transform.position) < FLOAT_MIN_ZOOMIN)
        {
            Camera.main.transform.position = vecLastPos;
        }
    }

    // 카메라 줌아웃
    public void ZoomOut()
    {
        Vector3 vecDir = Camera.main.transform.forward;
        float fZoomDist = m_fCamZoomSpd * Time.deltaTime;

        Vector3 vecLastPos = Camera.main.transform.position;
        Camera.main.transform.position -= vecDir * fZoomDist;

        if (Vector3.Distance(Camera.main.transform.position, transform.position) > FLOAT_MAX_ZOOMOUT)
        {
            Camera.main.transform.position = vecLastPos;
        }
    }

    // 카메라 시계방향으로 회전
    public void RotateClockwise()
    {
        transform.Rotate(Vector3.up, m_fCamRotateSpd * Time.deltaTime);
    }

    // 카메라 반시계방향으로 회전
    public void RotateCounterClockwise()
    {
        transform.Rotate(Vector3.up, -m_fCamRotateSpd * Time.deltaTime);
    }

    // 카메라 XZ경계범위 초과를 보정
    private void CorrectXZPosBoundary()
    {
        Vector3 vecPos = transform.position;

        if (vecPos.x > FLOAT_MAX_XPOS) vecPos.x = FLOAT_MAX_XPOS;
        if (vecPos.x < FLOAT_MIN_XPOS) vecPos.x = FLOAT_MIN_XPOS;
        if (vecPos.z > FLOAT_MAX_ZPOS) vecPos.z = FLOAT_MAX_ZPOS;
        if (vecPos.z < FLOAT_MIN_ZPOS) vecPos.z = FLOAT_MIN_ZPOS;

        m_fXpos = vecPos.x;
        m_fZpos = vecPos.z;
        transform.position = vecPos;
    }
}
