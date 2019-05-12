using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameCamera : MonoBehaviour {

    private Camera cam;
    public Vector3 camPos;
    public Vector3 lookAtPos;
    public Vector3 velocity;
    public float fMoveSpeed;

	private void Start ()
    {
        if (!Initialize()) Debug.Log("GameCamera Initialization has error.");
        
	}

	private void Update ()
    {
        UpdateCamTransform();
    }

    #region # INITIALIZATION #
    public bool Initialize()
    {
        bool bNoError = true;

        if ((cam = GetComponent<Camera>()) == null) bNoError &= false;

        camPos = gameObject.transform.position;
        lookAtPos = GameObject.Find("Main Game").GetComponent<MainScene>().objDummy.transform.position;
        lookAtPos.y = 1.8281f; // For y-axis -5.00 degree init
        velocity = Vector3.zero;
        fMoveSpeed = (lookAtPos - camPos).magnitude;

        return bNoError;
    }
    #endregion

    #region # CAMERA CONTROLL #
    private void UpdateCamTransform()
    {
        Vector3 distVec = camPos - gameObject.transform.position;

        if (camPos != gameObject.transform.position)
        {
            Vector3 dirVec = distVec.normalized;

            velocity = (dirVec * (fMoveSpeed * Time.deltaTime));
        }
        else
        {
            velocity = Vector3.zero;
        }

        if (velocity != Vector3.zero)
        {
            float fVelDist = velocity.magnitude;
            float fDist = distVec.magnitude;

            if (fDist < fVelDist)
            {
                transform.position = camPos;
            }
            else
            {
                transform.position += velocity;
            }
        }

        cam.transform.rotation = Quaternion.LookRotation(lookAtPos - camPos);
    }

    public void SetCameraMoveSpeed(float moveSpeed)
    {
        fMoveSpeed = moveSpeed;
    }

    public void SetCamera(Vector3 pos, Vector3 lookat)
    {
        camPos = pos;
        lookAtPos = lookat;
    }

    public void SetCamera(Vector3 pos, Vector3 lookat, float moveSpeed)
    {
        SetCamera(pos, lookat);
        SetCameraMoveSpeed(moveSpeed);
    }
    #endregion
}
