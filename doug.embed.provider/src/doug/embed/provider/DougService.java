package doug.embed.provider;

import doug.embed.services.IDougService;

public class DougService implements IDougService {

	@Override
	public String sayHi() {
		return "Hi from DougService";
	}
	
}
