package com.evtlogger.service.event;

import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.stereotype.Service;

import com.evtlogger.dao.EventDao;
import com.evtlogger.dao.UserDao;
import com.evtlogger.etc.ServiceReturn;
import com.evtlogger.mapper.EventMapper;
import com.evtlogger.service.user.BaseUserService;

@Service
@Qualifier("eventBoardService")
public class EventService implements IEventService {
	// [Class private constants]
	static final Logger logger = LoggerFactory.getLogger(EventService.class);
	static final int EVENT_PER_PAGE = 10; // 한 페이지에 보여줄 이벤트 개수

	// [Class private variables]
	@Autowired private BaseUserService	baseUserService;
	@Autowired private EventMapper eventMapper;

	// [Methods]
    /**
     * <p>경조사를 {@link EVENT_PER_PAGE}개씩 페이지로 가져옵니다.</p>
     * <p>{@link String} userJwt, {@link String} userId 둘중 한 값은 회원 식별을 위해 반드시 필요합니다.</p>
     * @param userJwt : Encrypted userJwt.
     * @param userId : Email for userId.
     * @param payType : 지출/수입 경조사 구분.
     * <pre>
     *  - "all" or null : 지출/수입 모두
     *  - "outgo" : 지출 경조사
     *  - "income" : 경조사 수입
     * </pre>
     * @param pageNum : 획득할 페이지 오프셋.
     * @return {@link ServiceReturn} code and response.
     * <pre> - response
     *      {@link List}<{@link EventDao}> "eventList" : 획득한 경조사 리스트
     * </pre>
     */
	@Override
	public ServiceReturn selectByPage(String userJwt, String userId, String payType, int pageNum) {
        // 파라미터 검사
		if (pageNum < 0 || pageNum > 10000) {
			logger.error("'pageNum' is less then 0 or bigger then 10000! (pageNum:" + pageNum + ")");
			return ServiceReturn.make("02001"); // 페이지 번호가 허용 범위를 벗어났습니다.
		}
		
		// Service - 회원정보 식별
		ServiceReturn checkUserSvcRt = baseUserService.checkUser(userJwt, userId);
		
		if (checkUserSvcRt == null || !checkUserSvcRt.checkResult()) {
			logger.error("'checkUserSvcRt' is null or return false! (checkUserSvcRt:" + checkUserSvcRt.toString() + ")");
			return checkUserSvcRt;
		}
		
        UserDao callerUserDao = (UserDao)checkUserSvcRt.get("selectedUserDao");
        userId = callerUserDao.getUserId();

		// 경조사조회 호출한 사람의 회원정보 획득하여 조회데이터 생성
		UserDao evtCallerUserDao = (UserDao)checkUserSvcRt.get("selectedUserDao");
		int userSeq = evtCallerUserDao.getUserSeq();
		long offset = EVENT_PER_PAGE * pageNum;
		int count = EVENT_PER_PAGE;
		
		// Mapper(Select) - 경조사목록 페이지별 조회
		List<EventDao> eventList = null;
		
		try {
		    eventList = eventMapper.select_event_by_offset_count(userSeq, payType, offset, count);
		}
		catch (Exception e) {
		    logger.error("Mapper Exception!", e);
		    return ServiceReturn.make("DB에서 경조사 조회에 실패했습니다."); // DB에서 경조사 조회에 실패했습니다.
		}
		
		if (eventList == null) {
		    logger.error("'eventList' is null!");
		    return ServiceReturn.make("02101"); // DB에서 경조사 조회에 실패했습니다.
		}
		
        // 로깅 및 성공결과 응답
		logger.info("Success event select by page. (userId:" + userId + ", payType: " + payType + 
		        ", offset:" + offset + ")");
		return ServiceReturn.make("eventList", eventList);
	}

    /**
     * <p>'evtSeq'에 해당하는 경조사 한 개를 반환합니다.</p>
     * <p>{@link String} userJwt, {@link String} userId 둘중 한 값은 회원 식별을 위해 반드시 필요합니다.</p>
     * @param userJwt : Encrypted userJwt.
     * @param userId : Email for userId.
     * @param evtSeq : 획득할 경조사의 시퀀스.
     * @return {@link ServiceReturn} code and response.
     * <pre> - response
     *      {@link EventDao} "selectedEventDao" : 획득한 경조사
     * </pre>
     */
	@Override
	public ServiceReturn selectBySeq(String userJwt, String userId, long evtSeq) {
        // Service - 회원정보 식별
        ServiceReturn checkUserSvcRt = baseUserService.checkUser(userJwt, userId);
        
        if (checkUserSvcRt == null || !checkUserSvcRt.checkResult()) {
            logger.error("'checkUserSvcRt' is null or return false! (checkUserSvcRt:" + checkUserSvcRt + ")");
            return checkUserSvcRt;
        }
        
        UserDao callerUserDao = (UserDao)checkUserSvcRt.get("selectedUserDao");
        userId = callerUserDao.getUserId();
		
		// Mapper(Select) 게시글 1건 조회
		EventDao selectedEventDao = null;
		
		try {
            if ((selectedEventDao = eventMapper.select_event(evtSeq)) == null) {
                return ServiceReturn.make("02003"); // 존재하지 않는 경조사 시퀀스입니다.
            }
		}
		catch (Exception e) {
		    logger.error("Mapper Exception!", e);
		    return ServiceReturn.make("DB에서 경조사 조회에 실패했습니다."); // DB에서 경조사 조회에 실패했습니다.
		}
        
        // 서비스 요청자와 경조사 작성자가 같은지 확인
        int callerUserSeq = callerUserDao.getUserSeq();
        int writerUserSeq = selectedEventDao.getEvtWriterUserSeq();
        
        if (callerUserSeq != writerUserSeq) {
           logger.error("'callerUserSeq' is NOT equal to 'writerUserSeq'! (callerUserSeq:" +
                   callerUserSeq + ", writerUserSeq:" + writerUserSeq);
           return ServiceReturn.make("02002"); // 서비스 요청자 시퀀스와 경조사 작성자 시퀀스가 일치하지 않습니다.
        }
		
        // 로깅 및 성공결과 응답
        logger.info("Success event select by seq (userId:" + userId + ", evtSeq:" + evtSeq + ")");
		return ServiceReturn.make("selectedEventDao", selectedEventDao);
	}

    /**
     * <p>'insertEventDao'의 데이터를 DB에 추가합니다.</p>
     * <p>{@link String} userJwt, {@link String} userId 둘중 한 값은 회원 식별을 위해 반드시 필요합니다.</p>
     * @param userJwt : Encrypted userJwt.
     * @param userId : Email for userId.
     * @param insertEventDao : 추가할 경조사 정보.
     * @return {@link ServiceReturn} code and response.
     */
	@Override
	public ServiceReturn insert(String userJwt, String userId, EventDao insertEventDao) {
	    // 파라미터 검사
	    if (insertEventDao == null) {
	        logger.error("'insertEventDao' is null!");
	        return ServiceReturn.make("02004"); // 경조사 추가를 위한 필수값이 비었습니다.
	    }

        // Service - 회원정보 식별
        ServiceReturn checkUserSvcRt = baseUserService.checkUser(userJwt, userId);
        
        if (checkUserSvcRt == null || !checkUserSvcRt.checkResult()) {
            logger.error("'checkUserSvcRt' is null or return false! (checkUserSvcRt:" + checkUserSvcRt + ")");
            return checkUserSvcRt;
        }

        // 경조사 정보 생성
		UserDao writerUserDao = (UserDao)checkUserSvcRt.get("selectedUserDao");
		insertEventDao.setEvtWriterUserSeq(writerUserDao.getUserSeq());		// 경조사 작성자 시퀀스
		insertEventDao.setEvtTgtUserSeq(-1);								// 경조사 대상 시퀀스

		if (!insertEventDao.validate(logger)) {
			logger.error("insertEventDao.validate() return false!");
			return ServiceReturn.make("02004"); // 경조사 추가를 위한 필수값이 비었습니다.
		}
		
		// Mapper(Insert) 경조사 추가
		try {
		    eventMapper.insert_event(insertEventDao);
		}
		catch (Exception e) {
		    logger.error("Mapper Exception!", e);
		    return ServiceReturn.make("02102"); // DB에서 경조사 추가에 실패했습니다.
		}
		
        // 로깅 및 성공결과 응답		
		logger.info("Success event insertion (userId:" + userId + ", insertEventDao.evtDate:" +
		        insertEventDao.getEvtDate() + "insertEventDao.payAmount:" + insertEventDao.getPayAmount() +
		        ", insertEventDao.evtTgtUserName:" + insertEventDao.getEvtTgtUserName() + ")");
        return ServiceReturn.make();
	}

    /**
     * <p>기존 경조사 데이터를 'updateEventDao'의 데이터로 수정합니다.</p>
     * <p>{@link String} userJwt, {@link String} userId 둘중 한 값은 회원 식별을 위해 반드시 필요합니다.</p>
     * @param userJwt : Encrypted userJwt.
     * @param userId : Email for userId.
     * @param updateEventDao : 수정할 경조사 정보.
     * @return {@link ServiceReturn} code and response.
     */
	@Override
	public ServiceReturn updateBySeq(String userJwt, String userId, EventDao updateEventDao) {
        // 파라미터 검사
	    if (updateEventDao == null) {
	        logger.error("'updateEventDao' is null!");
	        return ServiceReturn.make("02005"); // 경조사 수정을 위한 필수값이 비었습니다.
	    }
	    
	    // Service - 회원정보 식별
        ServiceReturn checkUserSvcRt = baseUserService.checkUser(userJwt, userId);
        
        if (checkUserSvcRt == null || !checkUserSvcRt.checkResult()) {
            logger.error("'checkUserSvcRt' is null or return false! (checkUserSvcRt:" + checkUserSvcRt + ")");
            return checkUserSvcRt;
        }
        
        UserDao callerUserDao = (UserDao)checkUserSvcRt.get("selectedUserDao");
        userId = callerUserDao.getUserId();
               
		// Service - 수정대상 경조사 데이터 조회
		ServiceReturn selectEventSvcRt = selectBySeq(null, userId, updateEventDao.getEvtSeq());
		
		if (!selectEventSvcRt.checkResult()) {
			logger.error("selectEventSvcRt.checkResult() return false!");
			return selectEventSvcRt;
		}
		
        // 서비스 요청자와 경조사 작성자가 같은지 확인
        EventDao selectedEventDao = (EventDao)selectEventSvcRt.get("selectedEventDao");
        int callerUserSeq = callerUserDao.getUserSeq();
        int writerUserSeq = selectedEventDao.getEvtWriterUserSeq();
        
        if (callerUserSeq != writerUserSeq) {
           logger.error("'callerUserSeq' is NOT equal to 'writerUserSeq'! (callerUserSeq:" +
                   callerUserSeq + ", writerUserSeq:" + writerUserSeq);
           return ServiceReturn.make("02002"); // 서비스 요청자 시퀀스와 경조사 작성자 시퀀스가 일치하지 않습니다.
        }
		
		// 수정할 데이터에 필요한 기존데이터 추가
		updateEventDao.setEvtSeq(selectedEventDao.getEvtSeq());	// 이벤트 시퀀스
		updateEventDao.setEvtWriterUserSeq(callerUserSeq);		// 이벤트 작성자
		
		// 수정전 유효성 검사
		if (!updateEventDao.validate(logger)) {
			logger.error("updateEventDao.validate() return false!");
			return ServiceReturn.make("02005"); // 경조사 수정을 위한 필수값이 비었습니다.
		}
		
		// Mapper(Update) - 경조사 수정
		try {
		    eventMapper.update_event(updateEventDao);
		}
		catch (Exception e) {
		    logger.error("Mapper Exception!", e);
		    return ServiceReturn.make("02103"); // DB에서 경조사 변경에 실패했습니다.
		}
		
        // 로깅 및 성공결과 응답
        logger.info("Success event update (userId:" + userId + ", updateEventDao.evtDate:" +
                updateEventDao.getEvtDate() + "updateEventDao.payAmount:" + updateEventDao.getPayAmount() +
                ", updateEventDao.evtTgtUserName:" + updateEventDao.getEvtTgtUserName() + ")");
        return ServiceReturn.make();
	}

    /**
     * <p>'evtSeq' 값을 가진 경조사를 삭제합니다.</p>
     * <p>{@link String} userJwt, {@link String} userId 둘중 한 값은 회원 식별을 위해 반드시 필요합니다.</p>
     * @param userJwt : Encrypted userJwt.
     * @param userId : Email for userId.
     * @param evtSeq : 삭제할 경조사 시퀀스.
     * @return {@link ServiceReturn} code and response.
     */
	@Override
	public ServiceReturn deleteBySeq(String userJwt, String userId, long evtSeq) {
        // 파라미터 검사
       if (evtSeq < 0) {
            logger.error("'evtSeq' less then zero! (evtSeq:" + evtSeq + ")");
            return ServiceReturn.make("02006"); // 경조사 시퀀스가 올바르지 않습니다.
        }
        
        // Service - 회원정보 식별
        ServiceReturn checkUserSvcRt = baseUserService.checkUser(userJwt, userId);
        
        if (checkUserSvcRt == null || !checkUserSvcRt.checkResult()) {
            logger.error("'checkUserSvcRt' is null or return false! (checkUserSvcRt:" + checkUserSvcRt + ")");
            return checkUserSvcRt;
        }
        
        UserDao callerUserDao = (UserDao)checkUserSvcRt.get("selectedUserDao");
        userId = callerUserDao.getUserId();
                
        // Service - 수정대상 경조사 데이터 조회
        ServiceReturn selectedEventSvcRt = selectBySeq(null, userId, evtSeq);
        
        if (!selectedEventSvcRt.checkResult()) {
            logger.error("selectedEventSvcRt.checkResult() return false!");
            return selectedEventSvcRt;
        }
		
        // 서비스 요청자와 경조사 작성자가 같은지 확인
        EventDao selectedEventDao = (EventDao)selectedEventSvcRt.get("selectedEventDao");
        int callerUserSeq = callerUserDao.getUserSeq();
        int writerUserSeq = selectedEventDao.getEvtWriterUserSeq();
        
        if (callerUserSeq != writerUserSeq) {
           logger.error("'callerUserSeq' is NOT equal to 'writerUserSeq'! (callerUserSeq:" +
                   callerUserSeq + ", writerUserSeq:" + writerUserSeq);
           return ServiceReturn.make("02002"); // 서비스 요청자 시퀀스와 경조사 작성자 시퀀스가 일치하지 않습니다.
        }
		
		// Mapper(Delete) - 경조사 삭제
        try {
            eventMapper.delete_event(evtSeq);
        }
        catch (Exception e) {
            logger.error("Mapper Exception!", e);
            return ServiceReturn.make("02104"); // DB에서 경조사 삭제에 실패했습니다.
        }
		
        // 로깅 및 성공결과 응답
        logger.info("Success event delete (userId:" + userId + ", evtSeq:" + evtSeq + ")");
        return ServiceReturn.make();
	}
}
