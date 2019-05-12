<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib uri="http://www.springframework.org/tags" prefix="spring" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
	<title><spring:message code="members.register.title"/></title>
</head>
<body>
	<p><spring:message code="members.register.welcome" arguments="${memberRegistrationCmd.memberNickname}"/></p>
	<p><a href="../main">[<spring:message code="name.page.main"/>]</a></p>
</body>
</html>