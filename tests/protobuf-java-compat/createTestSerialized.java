import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.InputStreamReader;
import java.io.IOException;
import java.io.PrintStream;

//import TestProto;

class createTestSerialized {

  public static void createInt32TestFiles() {

    try {

      System.out.println("createMinInt32");
      System.out.println(" example_int32=" + Integer.MIN_VALUE);

      TestProto.testInt32 minObj = TestProto.testInt32.newBuilder()
        .setExampleInt32(Integer.MIN_VALUE)
        .build();

      FileOutputStream minOutput = new FileOutputStream("min_int32.bin");
      minObj.writeTo(minOutput);
      minOutput.close();

      System.out.println("createMaxInt32");
      System.out.println(" example_int32=" + Integer.MAX_VALUE);

      TestProto.testInt32 maxObj = TestProto.testInt32.newBuilder()
        .setExampleInt32(Integer.MAX_VALUE)
        .build();

      FileOutputStream maxOutput = new FileOutputStream("max_int32.bin");
      maxObj.writeTo(maxOutput);
      maxOutput.close();

    } catch ( Exception e ) {
      e.printStackTrace();
    }

  }

  public static void createInt64TestFiles() {

    try {

      System.out.println("createMinInt64");
      System.out.println(" example_int64=" + Long.MIN_VALUE);

      TestProto.testInt64 minObj = TestProto.testInt64.newBuilder()
        .setExampleInt64(Long.MIN_VALUE)
        .build();

      FileOutputStream minOutput = new FileOutputStream("min_int64.bin");
      minObj.writeTo(minOutput);
      minOutput.close();

      System.out.println("createMaxInt64");
      System.out.println(" example_int64=" + Long.MAX_VALUE);

      TestProto.testInt64 maxObj = TestProto.testInt64.newBuilder()
        .setExampleInt64(Long.MAX_VALUE)
        .build();

      FileOutputStream maxOutput = new FileOutputStream("max_int64.bin");
      maxObj.writeTo(maxOutput);
      maxOutput.close();

    } catch ( Exception e ) {
      e.printStackTrace();
    }

  }

  public static void createUInt32TestFiles() {

    try {

      System.out.println("createMinUInt32");
      System.out.println(" example_uint32=" + 0);

      TestProto.testUInt32 minObj = TestProto.testUInt32.newBuilder()
        .setExampleUint32(0)
        .build();

      FileOutputStream minOutput = new FileOutputStream("min_uint32.bin");
      minObj.writeTo(minOutput);
      minOutput.close();

      System.out.println("createMaxUInt32");
      System.out.println(" example_uint32=" + 0xFFFFFFFF);

      TestProto.testUInt32 maxObj = TestProto.testUInt32.newBuilder()
        .setExampleUint32(0xFFFFFFFF)
        .build();

      FileOutputStream maxOutput = new FileOutputStream("max_uint32.bin");
      maxObj.writeTo(maxOutput);
      maxOutput.close();

    } catch ( Exception e ) {
      e.printStackTrace();
    }

  }

  public static void createUInt64TestFiles() {

    try {

      System.out.println("createMinUInt64");
      System.out.println(" example_uint64=" + 0);

      TestProto.testUInt64 minObj = TestProto.testUInt64.newBuilder()
        .setExampleUint64(0)
        .build();

      FileOutputStream minOutput = new FileOutputStream("min_uint64.bin");
      minObj.writeTo(minOutput);
      minOutput.close();

      System.out.println("createMaxUInt64");
      System.out.println(" example_uint64=" + Long.MAX_VALUE);

      TestProto.testUInt64 maxObj = TestProto.testUInt64.newBuilder()
        .setExampleUint64(Long.MAX_VALUE)
        .build();

      FileOutputStream maxOutput = new FileOutputStream("max_uint64.bin");
      maxObj.writeTo(maxOutput);
      maxOutput.close();

    } catch ( Exception e ) {
      e.printStackTrace();
    }

  }

  public static void createSInt32TestFiles() {

    try {

      System.out.println("createMinSInt32");
      System.out.println(" example_sint32=" + Integer.MIN_VALUE);

      TestProto.testSInt32 minObj = TestProto.testSInt32.newBuilder()
        .setExampleSint32(Integer.MIN_VALUE)
        .build();

      FileOutputStream minOutput = new FileOutputStream("min_sint32.bin");
      minObj.writeTo(minOutput);
      minOutput.close();

      System.out.println("createMaxSInt32");
      System.out.println(" example_sint32=" + Integer.MAX_VALUE);

      TestProto.testSInt32 maxObj = TestProto.testSInt32.newBuilder()
        .setExampleSint32(Integer.MAX_VALUE)
        .build();

      FileOutputStream maxOutput = new FileOutputStream("max_sint32.bin");
      maxObj.writeTo(maxOutput);
      maxOutput.close();

    } catch ( Exception e ) {
      e.printStackTrace();
    }

  }

  public static void createSInt64TestFiles() {

    try {

      System.out.println("createMinSInt64");
      System.out.println(" example_sint64=" + Long.MIN_VALUE);

      TestProto.testSInt64 minObj = TestProto.testSInt64.newBuilder()
        .setExampleSint64(Long.MIN_VALUE)
        .build();

      FileOutputStream minOutput = new FileOutputStream("min_sint64.bin");
      minObj.writeTo(minOutput);
      minOutput.close();

      System.out.println("createMaxSInt64");
      System.out.println(" example_sint64=" + Long.MAX_VALUE);

      TestProto.testSInt64 maxObj = TestProto.testSInt64.newBuilder()
        .setExampleSint64(Long.MAX_VALUE)
        .build();

      FileOutputStream maxOutput = new FileOutputStream("max_sint64.bin");
      maxObj.writeTo(maxOutput);
      maxOutput.close();

    } catch ( Exception e ) {
      e.printStackTrace();
    }

  }

  public static void createBoolTestFiles() {

    try {

      System.out.println("createMinBool");
      System.out.println(" example_bool=" + false);

      TestProto.testBool minObj = TestProto.testBool.newBuilder()
        .setExampleBool(false)
        .build();

      FileOutputStream minOutput = new FileOutputStream("min_bool.bin");
      minObj.writeTo(minOutput);
      minOutput.close();

      System.out.println("createMaxBool");
      System.out.println(" example_bool=" + true);

      TestProto.testBool maxObj = TestProto.testBool.newBuilder()
        .setExampleBool(true)
        .build();

      FileOutputStream maxOutput = new FileOutputStream("max_bool.bin");
      maxObj.writeTo(maxOutput);
      maxOutput.close();

    } catch ( Exception e ) {
      e.printStackTrace();
    }

  }

  public static void main(String[] args) throws Exception {

    // Walk through all the varint values and push out a individual test blob.
    createInt32TestFiles();
    createInt64TestFiles();
    createUInt32TestFiles();
    createUInt64TestFiles();
    createSInt32TestFiles();
    createSInt64TestFiles();
    createBoolTestFiles();

    /*
    System.out.println("started");
    try {

      // Set everthing to their mins.
      System.out.println("min_values");
      System.out.println(" example_uint32=" + 0);
      System.out.println(" example_uint64=" + 0);
      System.out.println(" example_sint32=" + Integer.MIN_VALUE);
      System.out.println(" example_sint64=" + Long.MIN_VALUE);
      System.out.println(" example_bool=" + false);

      ProtobufValidOuterClass.ProtobufValid minObj = ProtobufValidOuterClass.ProtobufValid.newBuilder()
        .setExampleUint32(0)
        .setExampleUint64(0)
        .setExampleSint32(Integer.MIN_VALUE)
        .setExampleSint64(Long.MIN_VALUE)
        .setExampleBool(false)
        .build();

      FileOutputStream minOutput = new FileOutputStream("min_values.bin");
      minObj.writeTo(minOutput);
      minOutput.close();

    } catch (IOException e) {
      e.printStackTrace();
    }
//
//      .setExampleInt64(0)
//      .build();

    */
  }

}
