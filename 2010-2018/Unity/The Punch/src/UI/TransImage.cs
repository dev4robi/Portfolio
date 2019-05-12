using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class TransImage : Image
{
    public Vector3 imgAnchoredPos;
    public float fMoveSpeed;
    private Vector3 velocity;
    private float fTargetAlpha;
    private float fAlphaDeltaBySec;
    private float fTargetScale;
    private float fScaleDeltaBySec;

    private void Update()
    {
        UpdateImgTransform();
        UpdateAlpha();
        UpdateScale();
    }

    #region # IMAGE CONTROLL #
    private void UpdateImgTransform()
    {
        Vector3 distVec = imgAnchoredPos - rectTransform.anchoredPosition3D;

        if (imgAnchoredPos != rectTransform.position)
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
                rectTransform.anchoredPosition3D = imgAnchoredPos;
            }
            else
            {
                rectTransform.anchoredPosition3D += velocity;
            }
        }
    }

    public void SetMoveSpeed(float moveSpeed)
    {
        fMoveSpeed = moveSpeed;
    }

    public void SetPosition(Vector3 pos)
    {
        imgAnchoredPos = pos;
    }

    public void SetPosition(Vector3 pos, float moveSpeed)
    {
        SetPosition(pos);
        SetMoveSpeed(moveSpeed);
    }

    private void UpdateAlpha()
    {
        float fUpdateAlpha = fAlphaDeltaBySec * Time.deltaTime;

        if (fUpdateAlpha < 0) // alpha-
        {
            float fUpdatedAlpha = color.a + fUpdateAlpha;

            if (fUpdatedAlpha < fTargetAlpha)
            {
                color = new Color(color.r, color.g, color.b, fTargetAlpha);
                fAlphaDeltaBySec = 0.00f;
            }
            else
            {
                color = new Color(color.r, color.g, color.b, fUpdatedAlpha);
            }
        }
        else if (fUpdateAlpha > 0) // alpha+
        {
            float fUpdatedAlpha = color.a + fUpdateAlpha;

            if (fUpdatedAlpha > fTargetAlpha)
            {
                color = new Color(color.r, color.g, color.b, fTargetAlpha);
                fAlphaDeltaBySec = 0.00f;
            }
            else
            {
                color = new Color(color.r, color.g, color.b, fUpdatedAlpha);
            }
        }
        else
        {
            fAlphaDeltaBySec = 0.00f;
        }
    }

    private void UpdateScale()
    {
        float fUpdateScale = fScaleDeltaBySec * Time.deltaTime;

        if (fUpdateScale < 0) // scale-
        {
            float fUpdatedScale = transform.localScale.x + fUpdateScale;

            if (fUpdatedScale < fTargetScale)
            {
                transform.localScale = new Vector3(fTargetScale, fTargetScale, 1.00f);
                fScaleDeltaBySec = 0.00f;
            }
            else
            {
                transform.localScale = new Vector3(fUpdatedScale, fUpdatedScale, 1.00f);
            }
        }
        else if (fUpdateScale > 0) // scale+
        {
            float fUpdatedScale = transform.localScale.x + fUpdateScale;

            if (fUpdatedScale > fTargetScale)
            {
                transform.localScale = new Vector3(fTargetScale, fTargetScale, 1.00f);
                fScaleDeltaBySec = 0.00f;
            }
            else
            {
                transform.localScale = new Vector3(fUpdatedScale, fUpdatedScale, 1.00f);
            }
        }
        else
        {
            fScaleDeltaBySec = 0.00f;
        }
    }

    public void ChangeAlpha(float time, float tgtAlpha) // tgtAlpha: 0.0~1.0 (0~255)
    {
        if (time == 0.00f)
        {
            fAlphaDeltaBySec = 0.00f;
            color = new Color(color.r, color.g, color.b, tgtAlpha);
        }
        else
        {
            fAlphaDeltaBySec = ((tgtAlpha - color.a) / time);
        }

        fTargetAlpha = tgtAlpha;    
    }

    public void ChangeScale(float time, float tgtScale)
    {
        if (time == 0.00f)
        {
            fScaleDeltaBySec = 0.00f;
            transform.localScale = new Vector3(tgtScale, tgtScale, 1.00f);
        }
        else
        {
            fScaleDeltaBySec = ((tgtScale - transform.localScale.x) / time);
        }

        fTargetScale = tgtScale;
    }
    #endregion
}
