package com.evtlogger.service.user;

import java.util.Calendar;
import java.util.Date;
import java.util.Map;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.stereotype.Service;

import com.evtlogger.dao.UserDao;
import com.evtlogger.etc.ServiceReturn;
import com.evtlogger.manager.UserJwtManager;
import com.evtlogger.mapper.UserMapper;
import com.evtlogger.util.StringUtil;

@Service
@Qualifier("baseUserService")
public class BaseUserService implements IUserService {
    // [Class private constants]
    private static final Logger logger = LoggerFactory.getLogger(BaseUserService.class);
    
    // [Class private variables]
    private @Autowired UserJwtManager userJwtManager;
    private @Autowired UserMapper userMapper;

    // [Methods]
    /**
     * <p>회원을 가입시킵니다.</p>
     * @param userDao : 새 회원 데이터.
     * @return {@link ServiceReturn} code and response.
     */
    @Override
    public ServiceReturn signup(UserDao userDao) {
        // 파라미터 검사
        if (userDao == null) {
            logger.error("'userDao' is null!");
            return ServiceReturn.make("01008"); // 회원가입 정보가 비었습니다.
        }
        
        String userId = userDao.getUserId();
        
        if (userId == null) {
            logger.error("'userId' is null!");
            return ServiceReturn.make("01001"); // 아이디가 필요합니다.
        }

        if (!StringUtil.isEmail(userId)) {
            logger.error("'userId' is NOT email type! (userId:" + userId + ")");
            return ServiceReturn.make("01006"); // 아이디가 이메일 형식이 아닙니다.
        }
        
        String userName = userDao.getUserName();
        
        if (userName == null || userName.length() == 0) {
            logger.error("'userName' is null or zero length! (userName:" + userName + ")");
            return ServiceReturn.make("01009"); // 회원명이 필요합니다.           
        }
        
        // Mapper(Select) - 유저 정보 가져옴 (중복가입 확인)
        UserDao selectedUserDao = userMapper.select_user(userId);

        if (selectedUserDao != null) { // 이미 가입된 회원
            if (selectedUserDao.getUserSignoutDate() == null) { // 탈퇴내역이 없는 회원
                    logger.error("Eamil already signined! (userId:" + userId +
                            ", userJoinDate:" + selectedUserDao.getUserJoinDate() + ")");
                    return ServiceReturn.make("01010"); // 이미 가입된 이메일입니다.
            }
        }
        
        // Mapper(Insert) - 회원정보 추가
        Date userJoinDate = Calendar.getInstance().getTime();
        userDao.setUserJoinDate(userJoinDate);
        userMapper.insert_user(userDao);
        
        // 로깅 및 성공결과 응답
        logger.info("Success new user signin. (userId: " +
                userDao.getUserId() + ", userName: " + userDao.getUserName() + ")");
        return null;
    }

    /**
     * <p>회원을 탈퇴시킵니다.</p>
     * @param userId : Email for userId
     * @return {@link ServiceReturn} code and response.
     */
    @Override
    public ServiceReturn deregist(String userJwt, String userId) {
        // 파라미터 검사
        if (userJwt == null && userId == null) {
            logger.error("Both 'userJwt' and 'userId' are null!");
            return ServiceReturn.make("01002"); // 토큰이나 아이디가 필요합니다.
        }
        
        // userJwt에서 userId추출
        if (userId == null) {
            Map<String, Object> userJwtMap = null;
            
            if ((userJwtMap = userJwtManager.decodeUserJwt(userJwt)) == null) {
                logger.error("'userJwtMap' is null!");
                return ServiceReturn.make("01003"); // 토큰으로 아이디 획득에 실패했습니다.
            }
            
            if ((userId = (String)userJwtMap.get("userId")) == null) {
                logger.error("'userId' is null!");
                return ServiceReturn.make("01004"); // 토큰에 아이디 정보가 없습니다.
            }
        }
        
        // Mapper(Update) - 유저 탈퇴일자 설정
        Date userSignoutDate = Calendar.getInstance().getTime();
        userMapper.update_user_signout(userId, userSignoutDate);
        
        // 로깅 및 성공결과 응답
        logger.info("Success user signout. (userId: " + userId + ")");
        return ServiceReturn.make();
    }

    /**
     * <p>회원을 로그인시킵니다.</p>
     * @param userId : Email for userId
     * @return {@link ServiceReturn} code and response.
     * <pre> - response
     *      "userJwt" : 로그인으로 생성된 userJwt
     * </pre>
     */
    @Override
    public ServiceReturn login(String userId) {
        // 파라미터 검사
        if (userId == null) {
            logger.error("'userId' is null!");
            return ServiceReturn.make("01001"); // 아이디가 필요합니다.
        }
        
        if (!StringUtil.isEmail(userId)) {
            logger.error("'userId' is NOT email type! (userId:" + userId + ")");
            return ServiceReturn.make("01006"); // 아이디가 이메일 형식이 아닙니다.
        }
        
        // Mapper(Update) - 유저 로그인일자 갱신
        Date userLastLoginDate = Calendar.getInstance().getTime();
        userMapper.update_user_login(userId, userLastLoginDate);
        
        // UserJwt 생성
        String userJwt = null;
        
        if ((userJwt = userJwtManager.encodeUserJwt(userId)) == null) {
            return ServiceReturn.make("01007"); // 유저 JWT생성에 실패했습니다.
        }
        
        // 로깅 및 성공결과 응답
        logger.info("Success user login. (userId: " + userId + ")");
        return ServiceReturn.make("userJwt", userJwt);
    }
    
    /**
     * <p>유저를 반환합니다.</p>
     * @param userId : Email for userId
     * @return {@link ServiceReturn} code and response.
     * <pre> - response
     *      "selectedUserDao" : 획득한 유저 정보
     * </pre>
     */
    @Override
    public ServiceReturn selectUser(String userId) {
        // 파라미터 검사
        if (userId == null) {
            logger.error("'userId' is null!");
            return ServiceReturn.make("01001"); // 아이디가 필요합니다.
        }

        // Mapper(Select) - 유저정보 조회
        UserDao selectedUserDao = null;
        
        if ((selectedUserDao = userMapper.select_user(userId)) == null) {
            logger.error("'selectedUserDao' is null!");
            return ServiceReturn.make("01101"); // DB에서 회원 조회에 실패했습니다.
        }
    
        // 로깅 및 성공결과 응답
        logger.info("Success user select. (userId: " + userId + ")");
        return ServiceReturn.make("selectedUserDao", selectedUserDao);
    }

    /**
     * <p>유저의 존재여부 및 탈퇴여부를 확인후 반환합니다.</p>
     * @param userJwt : Encrypted userJwt
     * @param userId : Email for userId
     * @return {@link ServiceReturn} code and response.
     * <pre> - response
     *      "selectedUserDao" : 획득한 유저 정보
     * </pre>
     */
    @Override
    public ServiceReturn checkUser(String userJwt, String userId) {
        // 파라미터 검사
        if (userJwt == null && userId == null) {
            logger.error("Both 'userJwt' and 'userId' are null!");
            return ServiceReturn.make("01002"); // 토큰이나 아이디가 필요합니다.
        }
        
        // userJwt에서 userId추출
        if (userId == null) {
            Map<String, Object> userJwtMap = null;
            
            if ((userJwtMap = userJwtManager.decodeUserJwt(userJwt)) == null) {
                logger.error("'userJwtMap' is null!");
                return ServiceReturn.make("01003"); // 토큰으로 아이디 획득에 실패했습니다.
            }
            
            if ((userId = (String)userJwtMap.get("userId")) == null) {
                logger.error("'userId' is null!");
                return ServiceReturn.make("01004"); // 토큰에 아이디 정보가 없습니다.
            }
        }
        
        // Mapper(Select) - 유저정보 조회
        UserDao selectedUserDao = null;
        
        if ((selectedUserDao = userMapper.select_user(userId)) == null) {
            logger.error("'selectedUserDao' is null!");
            return ServiceReturn.make("01101"); // DB에서 회원 조회에 실패했습니다.
        }
        
        // 탈퇴한 회원인지 확인
        if (selectedUserDao.getUserSignoutDate() != null) {
            logger.error("'userSignoutDate' is NOT null!");
            return ServiceReturn.make("01005"); // 탈퇴한 회원입니다.
        }
        
        // 로깅 및 성공결과 응답
        logger.info("Success user check. (userId: " + userId + ")");
        return ServiceReturn.make("selectedUserDao", selectedUserDao);
    }
}
