/* Implement this class. */

import java.util.Queue;
import java.util.concurrent.PriorityBlockingQueue;
import java.util.concurrent.atomic.AtomicInteger;

public class MyHost extends Host {
    static class FIFO implements Comparable<FIFO> {
        static final AtomicInteger next = new AtomicInteger();

        final Task task;
        final int id;

        FIFO(Task task) {
            this.task = task;
            this.id = next.getAndIncrement();
        }

        @Override
        public int compareTo(FIFO o) {
            if (this.task.getPriority() != o.task.getPriority()) return o.task.getPriority() - this.task.getPriority();
            return this.id - o.id;
        }
    }

    private boolean running = true;
    private final Queue<FIFO> tasks = new PriorityBlockingQueue<>();

    private final Object lock = new Object();

    private volatile Task pausedTask, currentTask, replacementTask;
    private volatile long start;

    @Override
    public void run() {
        while (running) {
            if (currentTask == null) {
                if (pausedTask != null) {
                    currentTask = pausedTask;
                    pausedTask = null;
                } else {
                    synchronized (tasks) {
                        if (tasks.isEmpty()) {
                            continue;
                        }
                        currentTask = tasks.remove().task;
                    }
                }
            }

            start = System.currentTimeMillis();
            if (currentTask.isPreemptible()) {
                long left = currentTask.getLeft();
                synchronized (lock) {
                    try {
                        lock.wait(left);
                        long diff = System.currentTimeMillis() - start;
                        left -= diff;
                        if (left <= 0) {
                            currentTask.finish();
                            currentTask = null;
                        } else {
                            currentTask.setLeft(left);
                            pausedTask = currentTask;
                            currentTask = replacementTask;
                        }
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            } else {
                try {
                    Thread.sleep(currentTask.getDuration());
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                currentTask.finish();
                currentTask = null;
            }
        }
    }

    @Override
    public void addTask(Task task) {
        synchronized (lock) {
            if (currentTask != null && task.getPriority() > currentTask.getPriority() && currentTask.isPreemptible()) {
                replacementTask = task;
                lock.notify();
            } else {
                tasks.add(new FIFO(task));
            }
        }
    }

    @Override
    public int getQueueSize() {
        int size = tasks.size();
        if (currentTask != null) {
            size++;
        }
        if (pausedTask != null) {
            size++;
        }
        return size;
    }

    @Override
    public long getWorkLeft() {
        long sum = 0;
        synchronized (tasks) {
            for (FIFO fifo : tasks) {
                sum += fifo.task.getLeft();
            }
        }
        if (currentTask != null) {
            long left = currentTask.getLeft();
            long diff = System.currentTimeMillis() - start;
            left -= diff;
            sum += left;
        }
        if (pausedTask != null) {
            sum += pausedTask.getLeft();
        }
        return sum;
    }

    @Override
    public void shutdown() {
        running = false;
    }
}
