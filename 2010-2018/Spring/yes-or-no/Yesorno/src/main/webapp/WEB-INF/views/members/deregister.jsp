<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://www.springframework.org/tags" prefix="spring" %>
<%@ taglib prefix="form" uri="http://www.springframework.org/tags/form" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
	<title><spring:message code="members.deregister.title"/></title>
</head>
<body>
	<h1><spring:message code="members.deregister.title"/></h1>
	<form:form action="./" method="DELETE">
		<p><spring:message code="members.email"/>: ${deregistrationMember.memberEmail}</p>
		<p><spring:message code="members.nickname"/>: ${deregistrationMember.memberNickname}</p>
		<p><spring:message code="members.joinDate"/>: ${deregistrationMember.memberJoinDate}</p>
		<p><spring:message code="members.lastLoginDate"/>: ${deregistrationMember.memberLastLoginDate}</p>
		<strong>
			<p><spring:message code="members.deregister.info1"/></p>
			<p><spring:message code="members.deregister.info2"/></p>
		</strong>
		<p>
			<input type="submit" value="<spring:message code="input.button.deregister"/>"/> 
			<a href="../main">[<spring:message code="name.page.main"/>]</a>
		</p>
	</form:form>
</body>
</html>