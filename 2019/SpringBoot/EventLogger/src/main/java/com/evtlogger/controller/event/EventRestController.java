package com.evtlogger.controller.event;

import java.util.Map;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestHeader;
import org.springframework.web.bind.annotation.RestController;

import com.evtlogger.dto.DeleteEventDto;
import com.evtlogger.dto.PostPutEventDto;
import com.evtlogger.etc.ServiceReturn;
import com.evtlogger.service.event.IEventService;

@RestController
public class EventRestController {
	// [Class private constants]
	private static final Logger logger = LoggerFactory.getLogger(EventRestController.class);
	
	// [Class private variables]
	@Autowired @Qualifier("eventService")
	private IEventService eventSvc;
	
	// [Methods]
	@GetMapping("/events/{payType}/page/{pageNum}")
	public Map<String, Object> getAllTypeEventListByPage(
	        @PathVariable("payType") String payType,
	        @PathVariable("pageNum") int pageNum,
	        @RequestHeader(required=true) String userJwt
	) {
	    ServiceReturn svcReturn = eventSvc.selectByPage(userJwt, null, payType, pageNum);
	    return (svcReturn == null ? null : svcReturn.toMap());
	}
   
   @PostMapping("/events") // 경조사 추가
   public Map<String, Object> insertEvent(
           @RequestBody(required=true) PostPutEventDto postEventDto
   ) {
       ServiceReturn svcReturn = eventSvc.insert(postEventDto.getUserJwt(), null, postEventDto.getEventDao());
       return (svcReturn == null ? null : svcReturn.toMap());
   }
   
   @PutMapping("/events") // 이벤트 업데이트
   public Map<String, Object> updateEvent(
           @RequestBody(required=true) PostPutEventDto putEventDto
   ) {    
       ServiceReturn svcReturn = eventSvc.updateBySeq(putEventDto.getUserJwt(), null, putEventDto.getEventDao());
       return (svcReturn == null ? null : svcReturn.toMap());
   }
   
   @DeleteMapping("/events") // 이벤트 삭제
   public Map<String, Object> deleteEvent(
           @RequestBody(required=true) DeleteEventDto deleteEventDto
   ) {
       ServiceReturn svcReturn = eventSvc.deleteBySeq(deleteEventDto.getUserJwt(), null, deleteEventDto.getEvtSeq());
       return (svcReturn == null ? null : svcReturn.toMap());
   }
}
