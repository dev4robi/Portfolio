using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameState : MonoBehaviour
{
    protected MainScene mainScene;        // The main game's main script
    protected PunchInputMgr inputMgr;     // Punch input manager
    protected bool bNextState = false;    // Switch for state change

    public void SetMainScene(MainScene scene)
    {
        mainScene = scene;
        inputMgr = mainScene.GetComponent<PunchInputMgr>();
    }

    public virtual void FinishState() {}
    protected virtual void ChangeNextState() {}
}
