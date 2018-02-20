using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GuObject : MonoBehaviour {

    public float delayTime;
    public GameObject centerObject;

    private float tm_finish;
    private float tm;
    private TrailRenderer m_trailRenderer;
    
	// Use this for initialization
	void Start () {
        tm_finish = delayTime / 8f * 7f;
        tm = 0f;

        m_trailRenderer = this.GetComponent<TrailRenderer>();
        m_trailRenderer.time = delayTime * 2f;
        m_trailRenderer.startWidth = 0.3f;
        m_trailRenderer.endWidth = 0.3f;

    }
	
	// Update is called once per frame
	void Update () {
 
	}

    private void FixedUpdate()
    {
        tm += Time.deltaTime;
        if( tm > tm_finish)
        {
            if (tm <= delayTime)
            {
                m_trailRenderer.startColor = Color.red;
                m_trailRenderer.endColor = Color.white;
            }
            else
            {
                tm = 0f;
                m_trailRenderer.startColor = Color.white;
                m_trailRenderer.endColor = Color.red;
            }
           
        }
        else
        {
            m_trailRenderer.startColor = Color.white;
            m_trailRenderer.endColor = Color.white;
        }

        if (centerObject != null)
        {
            //围绕某个点的旋转，参数分别是 1.需要围绕的点 2.旋转的轴向 3.旋转的速度
            this.transform.RotateAround(centerObject.transform.position, Vector3.forward, Time.deltaTime / delayTime * 360f);
        }
    }
}
