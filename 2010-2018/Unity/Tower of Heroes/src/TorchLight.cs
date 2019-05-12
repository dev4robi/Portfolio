using UnityEngine;
using System.Collections;

public class TorchLight : MonoBehaviour {

    public bool bEnable = true; // 사용 여부
    public float fIntensityMin = 1.1f;
    public float fIntensityMax = 1.5f;
    public float fStartDelay = 0.5f;

    private float fIntensitySpd = 5.00f;
    private float fIntensityCur;
    private bool bIncreasePhase = true;
    private Light lightComp = null;

	// Use this for initialization
	void Start () {
        fIntensityCur = fIntensityMin;
        lightComp = this.gameObject.GetComponent<Light>();
	}
	
	// Update is called once per frame
	void Update () {
        // 사용중
	    if (bEnable == true)
        {
            // 시작 지연 연산중
            if (fStartDelay > 0.00f)
            {
                fStartDelay -= Time.deltaTime;
            }
            // 시작 지연 연산 완료
            else
            {
                // 밝기 증가 페이즈
                if (bIncreasePhase == true)
                {
                    fIntensityCur += (fIntensitySpd * Time.deltaTime);
                    if (fIntensityCur >= fIntensityMax)
                    {
                        fIntensityCur = fIntensityMax;
                        bIncreasePhase = false;
                    }
                    
                }
                // 밝기 감소 페이즈
                else
                {
                    fIntensityCur -= (fIntensitySpd * Time.deltaTime);
                    if (fIntensityCur <= fIntensityMin)
                    {
                        fIntensityCur = fIntensityMin;
                        bIncreasePhase = true;
                    }
                }
            }

            // 연산된 밝기 적용
            lightComp.intensity = fIntensityCur;
        }
	}
}
