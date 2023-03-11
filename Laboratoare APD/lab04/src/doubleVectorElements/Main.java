package doubleVectorElements;

public class Main {

    public static void main(String[] args) {
        int N = 100000013;
        int[] v = new int[N];
        int nrOfThreads = 4;
        Thread[] t = new Thread[nrOfThreads];

        for (int i = 0; i < N; i++) {
            v[i] = i;
        }

//        // Parallelize me
//        for (int i = 0; i < N; i++) {
//            v[i] = v[i] * 2;
//        }

        for (int i = 0; i < nrOfThreads; i++) {
            t[i] = new Thread(new MyThread(i, N, nrOfThreads, v));
            t[i].start();
        }

        for (int i = 0; i < nrOfThreads; i++) {
            try {
                t[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        for (int i = 0; i < N; i++) {
            if (v[i] != i * 2) {
                System.out.println("Wrong answer");
                System.exit(1);
            }
        }
        System.out.println("Correct");
    }

}
