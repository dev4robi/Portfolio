package com.ghasix.datas.result;

import java.util.HashMap;
import java.util.Map;

import lombok.AccessLevel;
import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.ToString;

@AllArgsConstructor(access = AccessLevel.PROTECTED)
@Getter
@ToString
public class ApiResult {

    private static final String KEY_TID                = "tId";
    private static final String KEY_RESULT_CODE        = "resultCode";
    private static final String KEY_RESULT_MSG         = "resultMsg";
    private static final String KEY_RESULT_DATA        = "resultDatas";
    
    private static final String VAL_TID_DEFAULT        = null;
    private static final String VAL_CODE_SUCCESS       = "00000";
    private static final String VAL_CODE_SYSFAIL       = "99999";
    private static final String VAL_MSG_DEFAULT        = null;

    private Map<String, Object> apiResult; // resultMsg를 어떻게 채워넣을 것인가? ApiResultManager를 만들고, getInstance를 거기서만 가능하게? 음... @@

    private static ApiResult getInstance(String tId, String resultCode, String resultMsg, Map<String, Object> resultData) {
        tId         = (tId          == null ? VAL_TID_DEFAULT   : tId);
        resultCode  = (resultCode   == null ? VAL_CODE_SUCCESS  : resultCode);
        resultMsg   = (resultMsg    == null ? VAL_MSG_DEFAULT   : resultMsg);

        Map<String, Object> resultDataMap = null;

        if (resultData != null) {
            resultDataMap = new HashMap<String, Object>();
            resultDataMap.put(KEY_TID, tId);
            resultDataMap.put(KEY_RESULT_CODE, resultCode);
            resultDataMap.put(KEY_RESULT_MSG, resultMsg);
            resultDataMap.put(KEY_RESULT_DATA, resultData);
        }

        return new ApiResult(resultDataMap);
    }

    public static ApiResult make(String resultCode) {
        return getInstance(null, resultCode, null, null);
    }

    public static ApiResult make(Map<String, Object> resultData) {
        return getInstance(null, null, null, resultData);
    }

    public static ApiResult make(String resultCode, Map<String, Object> resultData) {
        return getInstance(null, resultCode, null, resultData);
    }

    public static ApiResult make(String resultCode, Object... resultDataKeyValAry) {
        if (resultDataKeyValAry == null || resultDataKeyValAry.length % 2 != 0) {
            return getInstance(null, VAL_CODE_SYSFAIL, null, null);
        }
        
        Map<String, Object> resultData = new HashMap<String, Object>();
        String key = null;
        Object val = null;

        for (int i = 0; i < resultDataKeyValAry.length; i += 2) {
            key = resultDataKeyValAry[i].toString();
            val = resultDataKeyValAry[i + 1];
            resultData.put(key, val);
        }

        return make(resultCode, resultData);
    }

    public static ApiResult make(Object... resultDataKeyValAry) {
        return make(null, resultDataKeyValAry);
    }

    public boolean checkResultCodeSuccess() {
        if (this.apiResult == null) {
            return false;
        }

        String resultCode = this.apiResult.get(KEY_RESULT_CODE).toString();

        if (resultCode == null) {
            return false;
        }

        return resultCode.equals(VAL_CODE_SUCCESS);
    }

    public void addResultData(Map<String, Object> addResultData) {
        if (this.apiResult == null || addResultData == null) {
            return;
        }

        Map<String, Object> resultDataMap = (Map<String, Object>) this.apiResult.get(KEY_RESULT_DATA);

        if (resultDataMap == null) {
            return;
        }

        resultDataMap.putAll(addResultData);
    }

    public Object getResultData(String key) {
        if (this.apiResult == null) {
            return null;
        }

        Map<String, Object> resultDataMap = (Map<String, Object>) this.apiResult.get(KEY_RESULT_DATA);

        if (resultDataMap == null) {
            return null;
        }

        return resultDataMap.get(key);
    }

    public void controllerCompact(String tId) {
        if (this.apiResult == null) {
            return;
        }

        this.apiResult.put(KEY_TID, tId);

        Map<String, Object> resultDataMap = (Map<String, Object>) this.apiResult.get(KEY_RESULT_DATA);

        if (resultDataMap != null) {
            if (resultDataMap.size() == 0) {
                this.apiResult.put(KEY_RESULT_DATA, null);
            }
        }
    }
}