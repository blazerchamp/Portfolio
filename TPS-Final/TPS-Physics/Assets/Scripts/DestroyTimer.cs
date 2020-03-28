﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DestroyTimer : MonoBehaviour
{
    public float destroyTime = 0.1f;
    // Start is called before the first frame update
    void Start()
    {
        StartCoroutine(DestroyRoutine());
    }

    IEnumerator DestroyRoutine()
    {
        yield return new WaitForSeconds(destroyTime);
        Destroy(gameObject);
    }
}