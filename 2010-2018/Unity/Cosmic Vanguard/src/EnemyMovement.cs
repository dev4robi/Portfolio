using UnityEngine;
using System.Collections;

public class EnemyMovement : MonoBehaviour {

    public GameObject Route;
    public float moveSpeed;
    int childCount = 0;
    public int currentNode = 0;
    int nextNode = 1;
    public bool loopMode = false;

    float slow_duration = 0;
    float slow_multiplier = 1;

    public float rotationSpeed;


	// Use this for initialization
	void Start () {
        if (Route == null)
        {
            Route = StaticManager.game.objWaypointNode;
        }

        childCount = Route.transform.childCount;
        if (childCount == 0)
        {
            Debug.Log("Child is Zero");
        }
        else
        {
            //Debug.Log("" + childCount);

            transform.position = Route.transform.GetChild(currentNode).transform.position;
        }
	}

    // Update is called once per frame
    void FixedUpdate()
    {

        if(slow_duration < 0)
        {
            slow_multiplier = 1;
        }
        else
        {
            slow_duration -= Time.deltaTime;
        }

        if (loopMode)
        {
            nextNode = (currentNode + 1) % childCount;
            currentNode = currentNode % childCount;
        }
        else
            nextNode = (currentNode + 1);

        if (nextNode < childCount)
        {


            transform.position = Vector3.MoveTowards(
                transform.position,
                Route.transform.GetChild(nextNode).transform.position,
                Time.deltaTime * moveSpeed * slow_multiplier);


            Quaternion rot = Quaternion.LookRotation(
            //transform.rotation = Quaternion.LookRotation(
                Route.transform.GetChild(nextNode).transform.position -
                Route.transform.GetChild(currentNode).transform.position
            );

            transform.rotation = Quaternion.RotateTowards(transform.rotation, rot, Time.deltaTime * rotationSpeed);

            if (transform.position == Route.transform.GetChild(nextNode).transform.position)
            {
                currentNode += 1;

            }
        }
    }

    public void doSlow(float multiplier)
    {
        slow_multiplier = multiplier;
        
    }


    public void doSlowDuration(float duration)
    {
        slow_duration = duration;

    }

    public void setCurrentNode(int nodeNum)
    {

        Debug.Log("111");
        currentNode = nodeNum;
        nextNode = nodeNum + 1;
        transform.position = Route.transform.GetChild(currentNode).transform.position;
    }


}
