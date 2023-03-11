package doubleVectorElements;

public class MyThread implements Runnable {
    int id, N, P, start, end;
    int[] v;

    public MyThread(int id, int n, int p, int[] v) {
        this.id = id;
        N = n;
        P = p;
        this.v = v;

        start = id * N / P;
        end = Integer.min((id + 1) * N / P, N);
    }

    @Override
    public void run() {
        for (int  i = start; i < end; i++) {
            v[i] *= 2;
        }
    }
}
