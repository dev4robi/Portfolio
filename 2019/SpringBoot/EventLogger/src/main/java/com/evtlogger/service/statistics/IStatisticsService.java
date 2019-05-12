package com.evtlogger.service.statistics;

import com.evtlogger.etc.ServiceReturn;

public interface IStatisticsService {
    // 경조사 전체통계
    public ServiceReturn statisticAll(String userJwt, String userId);
    // 특정 대상의 이름으로 통계
    public ServiceReturn statisticByName(String userJwt, String userId, String tgtName);
    // 특정 대상의 연락처로 통계
    public ServiceReturn statisticByPhoneNum(String userJwt, String userId, String tgtPhoneNum);
}
