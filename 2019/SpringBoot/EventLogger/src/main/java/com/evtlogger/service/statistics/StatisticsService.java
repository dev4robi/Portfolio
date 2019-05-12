package com.evtlogger.service.statistics;

import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.stereotype.Service;

import com.evtlogger.dao.EventDao;
import com.evtlogger.dao.UserDao;
import com.evtlogger.dto.StatisticsDto;
import com.evtlogger.etc.ServiceReturn;
import com.evtlogger.mapper.StatisticsMapper;
import com.evtlogger.service.event.EventService;
import com.evtlogger.service.user.BaseUserService;

@Service
@Qualifier("statisticsService")
public class StatisticsService implements IStatisticsService {
    // [Class private constants]
    private static final Logger logger = LoggerFactory.getLogger(EventService.class);

    // [Class private variables]
    @Autowired @Qualifier("baseUserService") private BaseUserService baseUserSvc;
    @Autowired private StatisticsMapper statisticsMpr;
    
    // [Methods]
    /**
     * <p>전체 경조사 통계를 가져옵니다.</p>
     * <p>{@link String} userJwt, {@link String} userId 둘중 한 값은 회원 식별을 위해 반드시 필요합니다.</p>
     * @param userJwt : Encrypted userJwt.
     * @param userId : Email for userId.
     * @return {@link ServiceReturn} code and response.
     * <pre> - response
     *      {@link List}<{@link StaticticsDto}> "statisticsList" : 경조사 통계 내용 리스트 
     * </pre>
     */
    @Override
    public ServiceReturn statisticAll(String userJwt, String userId) {
        // Service - 회원정보 식별
        ServiceReturn checkUserSvcRt = baseUserSvc.checkUser(userJwt, userId);
        
        if (checkUserSvcRt == null || !checkUserSvcRt.checkResult()) {
            logger.error("'checkUserSvcRt' is null or return false! (checkUserSvcRt:" + checkUserSvcRt.toString() + ")");
            return checkUserSvcRt;
        }
        
        UserDao callerUserDao = (UserDao)checkUserSvcRt.get("selectedUserDao");
        
        List<EventDao> eventDaoList = null;
        
        // Mapper(Select)
        try {
            eventDaoList = statisticsMpr.select_statistic_by_userSeq(callerUserDao.getUserSeq());
        }
        catch (Exception e) {
            logger.error("Mapper Exception!", e);
            return ServiceReturn.make("03101"); // DB에서 경조사 통계생성을 위한 조회에 실패했습니다.
        }
        
        // 통계 구하기
        List<StatisticsDto> statisticsDtoList = new LinkedList<StatisticsDto>();
        if (!calculateStatisticsForTotal(statisticsDtoList, eventDaoList)) {
            return ServiceReturn.make("03001"); // 경조사 통계를 구하는중 오류가 발생했습니다.
        }
        
        // 로깅 및 결과반환
        logger.info("Success 'statistics all' calculation. (userId:" + userId + ")");
        return ServiceReturn.make("statisticsList", statisticsDtoList);
    }

    /**
     * <p>'tgtName'에 연관된 통계 리스트를 가져옵니다.</p>
     * <p>{@link String} userJwt, {@link String} userId 둘중 한 값은 회원 식별을 위해 반드시 필요합니다.</p>
     * @param userJwt : Encrypted userJwt.
     * @param userId : Email for userId.
     * @param tgtName : 경조사 조회 대상의 이름.
     * @return {@link ServiceReturn} code and response.
     * <pre> - response
     *      {@link List}<{@link StaticticsDto}> "statisticsList" : 경조사 통계 내용 리스트
     * </pre>
     */
    @Override
    public ServiceReturn statisticByName(String userJwt, String userId, String tgtName) {
        // Service - 회원정보 식별
        ServiceReturn checkUserSvcRt = baseUserSvc.checkUser(userJwt, userId);
        
        if (checkUserSvcRt == null || !checkUserSvcRt.checkResult()) {
            logger.error("'checkUserSvcRt' is null or return false! (checkUserSvcRt:" + checkUserSvcRt.toString() + ")");
            return checkUserSvcRt;
        }
        
        UserDao callerUserDao = (UserDao)checkUserSvcRt.get("selectedUserDao");
        
        List<EventDao> eventDaoList = null;
        
        // Mapper(Select)
        try {
            eventDaoList = statisticsMpr.select_statistic_by_tgtUserName(callerUserDao.getUserSeq(), tgtName);
        }
        catch (Exception e) {
            logger.error("Mapper Exception!", e);
            return ServiceReturn.make("03101"); // DB에서 경조사 통계생성을 위한 조회에 실패했습니다.
        }
        
        // 통계 구하기
        List<StatisticsDto> statisticsDtoList = new LinkedList<StatisticsDto>();
        if (!calculateStatisticsForSearchGroup(statisticsDtoList, eventDaoList)) {
            return ServiceReturn.make("03001"); // 경조사 통계를 구하는중 오류가 발생했습니다.
        }
        
        // 로깅 및 결과반환
        logger.info("Success 'statistics by name' calculation. (userId:" + userId + ")");
        return ServiceReturn.make("statisticsList", statisticsDtoList);
    }

    /**
     * <p>'tgtPhoneNum'에 연관된 통계 리스트를 가져옵니다.</p>
     * <p>{@link String} userJwt, {@link String} userId 둘중 한 값은 회원 식별을 위해 반드시 필요합니다.</p>
     * @param userJwt : Encrypted userJwt.
     * @param userId : Email for userId.
     * @param tgtPhoneNum : 경조사 조회 대상의 연락처.
     * @return {@link ServiceReturn} code and response.
     * <pre> - response
     *      {@link List}<{@link StaticticsDto}> "statisticsList" : 경조사 통계 내용 리스트
     * </pre>
     */
    @Override
    public ServiceReturn statisticByPhoneNum(String userJwt, String userId, String tgtPhoneNum) {
        // ...
        return ServiceReturn.make("03001"); // 경조사 통계를 구하는중 오류가 발생했습니다.
    }
    
    // 전체 통계 구하기
    private boolean calculateStatisticsForTotal(List<StatisticsDto> outStatisticsDtoList, List<EventDao> eventDaoList) {
        // 파라미터 검사
        if (outStatisticsDtoList == null) {
            logger.error("'outStatisticsDtoList' is null!");
            return false;
        }
        
        if (eventDaoList == null) {
            logger.error("'eventDaoList' is null!");
            return false;
        }
        
        long totalIncome = 0L;
        long totalOutgo = 0L;
        
        for (EventDao eventDao : eventDaoList) {
            long payAmount = eventDao.getPayAmount();
                       
            if (payAmount > 0L) {
                totalIncome += payAmount;
            }
            else {
                payAmount -= (-payAmount);
            }
        }
        
        StatisticsDto statisticsDto = new StatisticsDto();
        
        statisticsDto.setTgtName(null);
        statisticsDto.setTotalIncome(totalIncome);
        statisticsDto.setTotalOutgo(totalOutgo);
        statisticsDto.setResultMsg(null);
        outStatisticsDtoList.add(statisticsDto);
        
        return true;
    }
    
    // 검색그룹에 따라 통계 구하기
    private boolean calculateStatisticsForSearchGroup(List<StatisticsDto> outStatisticsDtoList, List<EventDao> eventDaoList) {
        // 파라미터 검사
        if (outStatisticsDtoList == null) {
            logger.error("'outStatisticsDtoList' is null!");
            return false;
        }
        
        if (eventDaoList == null) {
            logger.error("'eventDaoList' is null!");
            return false;
        }
        
        // 통계 구하기
        Map<String, Long> incomeMap = new HashMap<String, Long>();
        Map<String, Long> outgoMap = new HashMap<String, Long>();
        Set<String> nameSet = new HashSet<String>();
        
        for (EventDao eventDao : eventDaoList) {
            long payAmount = eventDao.getPayAmount();
            String evtTgtUserName = eventDao.getEvtTgtUserName();
            
            nameSet.add(evtTgtUserName);
            
            if (payAmount > 0L) {
                Long savedIncome = incomeMap.get(evtTgtUserName);
            
                if (savedIncome != null) {
                    payAmount += savedIncome.longValue();
                }
            
                incomeMap.put(evtTgtUserName, payAmount);
            }
            else {
                payAmount = -(payAmount);
                Long savedOutgo = outgoMap.get(evtTgtUserName);
                
                if (savedOutgo != null) {
                    payAmount += savedOutgo.longValue();
                }
            
                outgoMap.put(evtTgtUserName, payAmount);
            }
        }
        
        for (String name : nameSet) {
            Long income = incomeMap.get(name);
            Long outgo = outgoMap.get(name);
            StatisticsDto statisticsDto = new StatisticsDto();
            
            statisticsDto.setTgtName(name);
            statisticsDto.setTotalIncome(income == null ? 0 : income.longValue());
            statisticsDto.setTotalOutgo(outgo == null ? 0 : outgo.longValue());
            statisticsDto.setResultMsg(null);
            outStatisticsDtoList.add(statisticsDto);
        }

        incomeMap.clear();
        outgoMap.clear();
        nameSet.clear();
        return true;
    }
}
