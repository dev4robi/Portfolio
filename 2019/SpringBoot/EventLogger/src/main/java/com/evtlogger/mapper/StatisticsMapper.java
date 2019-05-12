package com.evtlogger.mapper;

import java.util.List;

import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;

import com.evtlogger.dao.EventDao;

@Mapper
public interface StatisticsMapper {

    // 작성자 userId에 해당하는 모든 경조사 조회 
    public List<EventDao> select_statistic_by_userSeq(@Param("userSeq") int userSeq);
    // 작성자 userId에 해당하고 대상의 이름이 tgtUserName인 모든 경조사 조회 
    public List<EventDao> select_statistic_by_tgtUserName(@Param("userSeq") int userSeq,
                                                          @Param("tgtUserName") String tgtUserName);
    // 작성자 userId에 해당하고 대상의 연락처가 tgtPhoneNum인 모든 경조사 조회
    public List<EventDao> select_statistic_by_tgtPhoneNum(@Param("userSeq") int userSeq,
                                                          @Param("tgtPhoneNum") String tgtPhoneNum);
}
