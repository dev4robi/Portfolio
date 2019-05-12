package com.evtlogger.auth;

import java.util.Map;

public interface IAuthenticator {

	public Map<String, Object> authenticate(String idToken);
}
