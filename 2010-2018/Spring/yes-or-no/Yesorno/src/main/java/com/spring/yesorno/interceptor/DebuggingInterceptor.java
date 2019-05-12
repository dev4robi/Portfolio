package com.spring.yesorno.interceptor;

import java.util.Date;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.web.servlet.ModelAndView;
import org.springframework.web.servlet.handler.HandlerInterceptorAdapter;

public class DebuggingInterceptor extends HandlerInterceptorAdapter {

	protected Log log = LogFactory.getLog(DebuggingInterceptor.class);

	@Override // 컨트롤러(핸들러) 실행 전
	public boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler) throws Exception {
		if (log.isDebugEnabled()) {
			log.debug("============================== [BEGIN] ==============================(Time: " + new Date().getTime() + ")");
			log.debug("[Request RUI\t: " + request.getRequestURI() + "]");
		}

		return super.preHandle(request, response, handler); // false반환 시 컨트롤(핸들러) 실행하지 않음
	}

	@Override // 컨트롤러(핸들러) 실행 후 (컨트롤러 Exception발생 시 실행하지 않음)
	public void postHandle(HttpServletRequest request, HttpServletResponse response, Object handler, ModelAndView modelAndView) throws Exception {
		if (log.isDebugEnabled()) {
			log.debug("============================== [E N D] ==============================(Time: " + new Date().getTime() + ")\n");
		}
	}
	
	/* @Override // 뷰 생성 직후 (컨트롤러 실행 이후 예외가 4번째 인자로 전달됨)
	public void afterCompletion(HttpServletRequest request, HttpServletResponse response, Object handler, Exception e) throws Exception {
		
	}*/

	/* [Note]
	 * 01. Client Request
	 * 02. DispatcherServlet
	 * 03. HandlerMapping
	 * 04. Interceptor - preHandle
	 * 05. Controller -> Cmd -> Svc -> Dao
	 * 06. Interceptor - postHandle
	 * 07. DispatcherServlet
	 * 08. ViewResolver
	 * 09. View
	 * 10. DispatcherServlet
	 * 12. Client에게 View 전송
	 * 11. Interceptor - afterCompletion
	 * 12. Client Response
	 */
}
