package com.evtlogger.service.event;

import com.evtlogger.dao.EventDao;
import com.evtlogger.etc.ServiceReturn;

public interface IEventService {

	public ServiceReturn selectByPage(String userJwt, String userId, String payType, int pageNum); // 경조사 페이지번호로 다중 조회
	public ServiceReturn selectBySeq(String userJwt, String userId, long evtSeq);                  // 경조사 시퀀스로 한건 조회
	public ServiceReturn insert(String userJwt, String userId, EventDao insertEventDao);           // 경조사 추가
	public ServiceReturn updateBySeq(String userJwt, String userId, EventDao updateEventDao);      // 경조사 수정
	public ServiceReturn deleteBySeq(String userJwt, String userId, long evtSeq);                  // 경조사 삭제
}
