package com.evtlogger.mapper;

import java.util.List;

import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;

import com.evtlogger.dao.EventDao;

@Mapper
public interface EventMapper {
	// 오프셋 + 개수로 경조사 여러개 조회
	public List<EventDao> select_event_by_offset_count(	@Param("userSeq") int userSeq,
	                                                    @Param("payType") String payType,
	                                                    @Param("offset") long offset,
														@Param("count") int count);
	// 경조사 1건 조회
	public EventDao select_event(@Param("evtSeq") long evtSeq);
	// 경조사 추가
	public void insert_event(EventDao insertEventDao);
	// 경조사 수정
	public void update_event(EventDao updateEventDao);
	// 경조사 삭제
	public void delete_event(@Param("evtSeq") long evtSeq);
}
