/* Implement this class. */

import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;

public class MyDispatcher extends Dispatcher {

    public MyDispatcher(SchedulingAlgorithm algorithm, List<Host> hosts) {
        super(algorithm, hosts);
    }

    // For Round Robin
    private AtomicInteger nextHost = new AtomicInteger();

    @Override
    public void addTask(Task task) {
        switch (algorithm) {
            case ROUND_ROBIN -> {
                int host = nextHost.getAndIncrement() % hosts.size();
                hosts.get(host).addTask(task);
            }
            case SHORTEST_QUEUE -> {
                synchronized (hosts) {
                    Host minHost = null;
                    int min = Integer.MAX_VALUE;
                    for (Host host : hosts) {
                        int queueSize = host.getQueueSize();
                        if (queueSize < min) {
                            min = queueSize;
                            minHost = host;
                        }
                    }
                    if (minHost != null) minHost.addTask(task);
                }
            }
            case SIZE_INTERVAL_TASK_ASSIGNMENT -> {
                switch (task.getType()) {
                    case SHORT -> hosts.get(0).addTask(task);
                    case MEDIUM -> hosts.get(1).addTask(task);
                    case LONG -> hosts.get(2).addTask(task);
                }
            }
            case LEAST_WORK_LEFT -> {
                synchronized (hosts) {
                    Host minHost = null;
                    long min = Integer.MAX_VALUE;
                    for (Host host : hosts) {
                        long workLeft = host.getWorkLeft();
                        if (workLeft < min) {
                            min = workLeft;
                            minHost = host;
                        }
                    }
                    if (minHost != null) minHost.addTask(task);
                }
            }
        }
    }
}
