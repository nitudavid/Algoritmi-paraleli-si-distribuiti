package Task1;

public class Main {
    public static void main(String[] args) {
        int nrOfProcessors = Runtime.getRuntime().availableProcessors();
        Thread[] t = new Thread[nrOfProcessors];

        for (int i = 0; i < nrOfProcessors; i++) {
            t[i] = new Thread(new MyThread(i));
            t[i].start();
        }

        for (int i = 0; i < nrOfProcessors; i++) {
            try {
                t[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
