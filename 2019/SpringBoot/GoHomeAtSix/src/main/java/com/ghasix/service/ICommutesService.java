package com.ghasix.service;

import com.ghasix.datas.dto.PostCommutesDto;
import com.ghasix.datas.dto.PutCommutesDto;
import com.ghasix.datas.result.ApiResult;

public interface ICommutesService {

    public ApiResult selectCommutesAll(String userJwt);
    public ApiResult selectCommutesById(String userJwt, long commuteId);
    public ApiResult selectCommutesByTime(String userJwt, long beginTime, long EndTime);
    public ApiResult insertCommutes(String userJwt, PostCommutesDto postCommutesDto);
    public ApiResult updateCommutes(String userJwt, PutCommutesDto updateCommutesDto);
    public ApiResult deleteCommutes(String userJwt);
}