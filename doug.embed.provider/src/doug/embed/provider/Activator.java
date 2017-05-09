package doug.embed.provider;

import org.osgi.framework.BundleActivator;
import org.osgi.framework.BundleContext;

import doug.embed.services.IDougService;

public class Activator implements BundleActivator {

	@Override
	public void start(BundleContext context) throws Exception {
		context.registerService(IDougService.class, new DougService(), null);
	}

	@Override
	public void stop(BundleContext context) throws Exception {
	}

}
