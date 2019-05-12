using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class TransText : MonoBehaviour
{
    public Text text;
    public Outline textOutline;
    public Vector3 imgPos;
    public Vector3 velocity;
    private float fTargetAlpha;
    private float fAlphaDeltaBySec;
    private int iTargetFontSize;
    private float fSizeDeltaBySec;
    private float fTargetScale;
    private float fScaleDeltaBySec;

    private void Start()
    {
        if (text == null) text = GetComponent<Text>();
        if (textOutline == null) textOutline = GetComponent<Outline>();
    }

    private void Update()
    {
        UpdateAlpha();
        UpdateFontSize();
        UpdateScale();
    }

    private void UpdateAlpha()
    {
        float fUpdateAlpha = fAlphaDeltaBySec * Time.deltaTime;

        if (fUpdateAlpha < 0) // alpha-
        {
            float fUpdatedAlpha = text.color.a + fUpdateAlpha;

            if (fUpdatedAlpha < fTargetAlpha)
            {
                text.color = new Color(text.color.r, text.color.g, text.color.b, fTargetAlpha);
                if (textOutline != null) textOutline.effectColor = new Color(textOutline.effectColor.r, textOutline.effectColor.g, textOutline.effectColor.b, fTargetAlpha);
                fAlphaDeltaBySec = 0.00f;
            }
            else
            {
                text.color = new Color(text.color.r, text.color.g, text.color.b, fUpdatedAlpha);
                if (textOutline != null) textOutline.effectColor = new Color(textOutline.effectColor.r, textOutline.effectColor.g, textOutline.effectColor.b, fUpdatedAlpha);
            }
        }
        else if (fUpdateAlpha > 0) // alpha+
        {
            float fUpdatedAlpha = text.color.a + fUpdateAlpha;

            if (fUpdatedAlpha > fTargetAlpha)
            {
                text.color = new Color(text.color.r, text.color.g, text.color.b, fTargetAlpha);
                if (textOutline != null) textOutline.effectColor = new Color(textOutline.effectColor.r, textOutline.effectColor.g, textOutline.effectColor.b, fTargetAlpha);
                fAlphaDeltaBySec = 0.00f;
            }
            else
            {
                text.color = new Color(text.color.r, text.color.g, text.color.b, fUpdatedAlpha);
                if (textOutline != null) textOutline.effectColor = new Color(textOutline.effectColor.r, textOutline.effectColor.g, textOutline.effectColor.b, fUpdatedAlpha);
            }
        }
        else
        {
            fAlphaDeltaBySec = 0.00f;
        }
    }

    private void UpdateFontSize()
    {
        float fUpdateSize = fSizeDeltaBySec * Time.deltaTime;

        if (fUpdateSize < 0) // size-
        {
            float fUpdatedSize = text.fontSize + fUpdateSize;

            if (fUpdatedSize < iTargetFontSize)
            {
                text.fontSize = iTargetFontSize;
                fSizeDeltaBySec = 0.00f;
            }
            else
            {
                text.fontSize = (int)fUpdatedSize;
            }
        }
        else if (fUpdateSize > 0) // size+
        {
            float fUpdatedSize = text.fontSize + fUpdateSize;

            if (fUpdatedSize > iTargetFontSize)
            {
                text.fontSize = iTargetFontSize;
                fSizeDeltaBySec = 0.00f;
            }
            else
            {
                text.fontSize = (int)fUpdatedSize;
            }
        }
        else
        {
            fSizeDeltaBySec = 0.00f;
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
            text.color = new Color(text.color.r, text.color.g, text.color.b, tgtAlpha);
            if (textOutline != null) textOutline.effectColor = new Color(textOutline.effectColor.r, textOutline.effectColor.g, textOutline.effectColor.b, tgtAlpha);
        }
        else
        {
            fAlphaDeltaBySec = ((tgtAlpha - text.color.a) / time);
        }

        fTargetAlpha = tgtAlpha;
    }

    public void ChangeSize(float time, int tgtSize) // tgtSize: 0~300
    {
        if (time == 0.00f)
        {
            fSizeDeltaBySec = 0.00f;
            text.fontSize = tgtSize;
        }
        else
        {
            fSizeDeltaBySec = ((float)(tgtSize - text.fontSize) / time);
        }

        iTargetFontSize = tgtSize;
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
}
